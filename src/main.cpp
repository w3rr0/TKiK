#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cfloat>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>
#include "storage/Database.hpp"
#include "ast/Statement.hpp"

// for Flex/Bison
extern int yyparse();
extern void yyrestart(FILE*);
typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(const char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern Statement* root_statement;

// global database and gui logs
Database db("test_database");
std::vector<std::vector<std::string>> gui_results;
std::vector<std::string> gui_headers;
std::string gui_error = "";
std::vector<std::string> gui_log;

const std::string DB_FILENAME = "database.bin";

int main() {
    if (!glfwInit()) return 1;
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "SQL Interpreter", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 2.0f;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // LOADING database at the beggining
    try {
        if (std::filesystem::exists(DB_FILENAME)) {
            db.loadFromFile(DB_FILENAME);
            gui_log.push_back("Database loaded from " + DB_FILENAME);
        } else {
            gui_log.push_back("No existing database file found. Starting fresh.");
        }
    } catch (const std::exception& e) {
        gui_log.push_back("Error loading database: " + std::string(e.what()));
    }

    char sql_input[10024] = "";

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("MainLayout", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

        ImGui::Columns(2, "MainColumns", true);
        ImGui::SetColumnWidth(0, ImGui::GetIO().DisplaySize.x * 0.75f);

        // left part with console, log and table display
        ImGui::BeginChild("WorkspaceChild");

        // CONSOLE
        ImGui::Text("SQL Terminal (Press CTRL+ENTER to execute) >");
        ImGui::SetWindowFontScale(1.4f);

        float input_height = ImGui::GetIO().DisplaySize.y * 0.30f;

        // Multiline input
        ImGui::InputTextMultiline("##query", sql_input, IM_ARRAYSIZE(sql_input),
                                     ImVec2(-FLT_MIN, input_height),
                                     ImGuiInputTextFlags_AllowTabInput);

        bool is_input_active = ImGui::IsItemActive();

        ImGui::SetWindowFontScale(1.0f);

        bool execute_pressed = ImGui::Button("Execute Query", ImVec2(-FLT_MIN, 45));

        // CTRL + ENTER
        bool ctrl_pressed = ImGui::GetIO().KeyCtrl || ImGui::GetIO().KeySuper;
        bool enter_shortcut = is_input_active && ctrl_pressed && ImGui::IsKeyPressed(ImGuiKey_Enter);

        // if clicked button or pressed CTRL+ENTER
        if (execute_pressed || enter_shortcut) {
            if (strlen(sql_input) > 0) {
                gui_error = "";
                gui_log.push_back("Executing: " + std::string(sql_input));

                YY_BUFFER_STATE buffer = yy_scan_string(sql_input);
                if (yyparse() == 0 && root_statement) {
                    try {
                        root_statement->execute();

                        // SAVING table to database
                        db.saveToFile(DB_FILENAME);
                        // clearing buffor
                        memset(sql_input, 0, sizeof(sql_input));
                        ImGui::SetKeyboardFocusHere(-1);

                    } catch (const std::exception& e) {
                        gui_error = e.what();
                        gui_log.push_back("Error: " + std::string(e.what()));
                    }
                    delete root_statement;
                    root_statement = nullptr;
                } else {
                    gui_error = "Syntax Error!";
                    gui_log.push_back("Error: Syntax Error!");
                    yyrestart(stdin);
                }
                yy_delete_buffer(buffer);
            }
        }

        // HISTORY LOG
        ImGui::Separator();
        ImGui::Text("History Log:");
        ImGui::BeginChild("LogRegion", ImVec2(0, ImGui::GetIO().DisplaySize.y * 0.15f), true);
        for (const auto& log_line : gui_log) {
            if (log_line.find("Error") != std::string::npos)
                ImGui::TextColored(ImVec4(1, 0.4f, 0.4f, 1), "%s", log_line.c_str());
            else if (log_line.find("Executing") != std::string::npos)
                ImGui::TextColored(ImVec4(0.4f, 0.8f, 1, 1), "%s", log_line.c_str());
            else
                ImGui::TextUnformatted(log_line.c_str());
        }
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);
        ImGui::EndChild();

        // TABLE DISPLAY
        ImGui::Separator();
        ImGui::Text("Table Results:");
        if (!gui_headers.empty() && ImGui::BeginTable("ResultsTable", (int)gui_headers.size(), ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_Resizable)) {

            for (const auto& h : gui_headers) {
                // UUPERCASE names of columns
                std::string upperHeader = h;
                std::transform(upperHeader.begin(), upperHeader.end(), upperHeader.begin(), ::toupper);
                ImGui::TableSetupColumn(upperHeader.c_str());
            }

            ImGui::TableHeadersRow();
            for (const auto& row : gui_results) {
                ImGui::TableNextRow();
                for (const auto& cell : row) {
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", cell.c_str());
                }
            }
            ImGui::EndTable();
        }
        ImGui::EndChild();

        // RIGHT COLUMN with database schema
        ImGui::NextColumn();
        ImGui::BeginChild("SchemaBrowser");
        ImGui::TextColored(ImVec4(0.2f, 0.3f, 0.55f, 1.0f), "Database schema");
        ImGui::Separator();

        std::vector<std::string> tableNames = db.getTableNames();
        if (tableNames.empty()) {
            ImGui::TextDisabled("(No tables)");
        } else {
            for (const auto& tName : tableNames) {
                if (ImGui::CollapsingHeader(tName.c_str())) {
                    try {
                        Table& t = db.getTable(tName);
                        const auto& cols = t.getColumns();
                        ImGui::Indent();
                        for (const auto& col : cols) {
                            ImGui::BulletText("%s (%s)", col.getName().c_str(), col.getTypeString().c_str());
                        }
                        ImGui::Unindent();
                    } catch (...) {
                        ImGui::TextColored(ImVec4(1,0,0,1), "Error loading columns");
                    }
                }
            }
        }
        ImGui::EndChild();

        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}