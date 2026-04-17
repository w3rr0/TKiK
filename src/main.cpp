#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <cfloat>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <filesystem>
#include <sstream>
#include <regex>
#include "storage/Database.hpp"
#include "ast/Statement.hpp"
#include "gui/Utils.hpp"

// Flex/Bison declaration to connect the parser with cpp
extern int yyparse();
extern void yyrestart(FILE*);
typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern YY_BUFFER_STATE yy_scan_string(const char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern Statement* root_statement;

// global data structures
const std::string DB_FILENAME = "database.bin";
Database db("test_database");
std::vector<std::vector<std::string>> gui_results;
std::vector<std::string> gui_headers;
std::string gui_error = "";
std::vector<std::string> gui_log;

// method that executes the ast statement
void executeSQL(const char* input) {
    gui_error = ""; gui_results.clear(); gui_headers.clear();
    gui_log.push_back("Executing: " + std::string(input));

    YY_BUFFER_STATE buffer = yy_scan_string(input);
    if (yyparse() == 0 && root_statement) {
        try {
            // execute the logic defined in Statement classes
            root_statement->execute();
            db.saveToFile(DB_FILENAME);
        } catch (const std::exception& e) {
            gui_error = e.what();
            gui_log.push_back("Error: " + std::string(e.what()));
        }
        delete root_statement; root_statement = nullptr;
    } else {
        gui_log.push_back("Error: Syntax Error!");
        yyrestart(stdin);
    }
    yy_delete_buffer(buffer);
}

int main(int argc, char** argv) {
    // loading database from file at startup
    try {
        if (std::filesystem::exists(DB_FILENAME)) {
            db.loadFromFile(DB_FILENAME);
        }
    } catch (...) {}

    bool use_gui = false;
    // checking for --gui flag in arguments
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--gui") use_gui = true;
    }
    // CLI version
    if (!use_gui){
        Utils::runCLI();
        return 0;
    }

    // initializing GLFW
    if (!glfwInit()) return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // setting up the window

    GLFWwindow* window = glfwCreateWindow(1280, 720, "sqW", NULL, NULL);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION(); ImGui::CreateContext();
    // font size in gui
    ImGui::GetIO().FontGlobalScale = 1.4f;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    char sql_input[10024] = "";

    // MAIN CONSOLE layout
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplGlfw_NewFrame(); ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        ImGui::Begin("MainLayout", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

        // dividing UI into two columns: Workspace and Database schema
        ImGui::Columns(2, "MainColumns", true);
        ImGui::SetColumnWidth(0, ImGui::GetIO().DisplaySize.x * 0.75f);

        ImGui::BeginChild("WorkspaceChild");

        ImGui::Text("sqW Terminal (Press CTRL+ENTER to execute) >");

        ImGuiIO& io = ImGui::GetIO();

        // CTRL + ENTER for fast execution of the query
        bool ctrl_pressed = io.KeyCtrl || io.KeySuper;
        bool enter_pressed = ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter);
        bool shortcut_pressed = ctrl_pressed && enter_pressed;

        ImGui::SetWindowFontScale(1.4f);
        float input_height = ImGui::GetIO().DisplaySize.y * 0.30f;

        ImGui::InputTextMultiline("##query", sql_input, IM_ARRAYSIZE(sql_input), ImVec2(-FLT_MIN, input_height), ImGuiInputTextFlags_AllowTabInput);


        bool is_input_focused = ImGui::IsItemFocused();

        // if triggered
        if (ImGui::Button("Execute Query", ImVec2(-FLT_MIN, 45)) || (is_input_focused && shortcut_pressed)) {
            if (strlen(sql_input) > 0) {
                executeSQL(sql_input);
                memset(sql_input, 0, sizeof(sql_input));
                ImGui::SetKeyboardFocusHere(-1); // going back to the input box
            }
        }

        // HISTORY LOG layout
        ImGui::Separator();
        ImGui::Text("History Log:");
        ImGui::BeginChild("LogRegion", ImVec2(0, ImGui::GetIO().DisplaySize.y * 0.15f), true);
        for (const auto& log_line : gui_log) {
            if (log_line.find("Executing:") != std::string::npos) {
                ImGui::TextColored(GUI_PINK, "Executing: ");
                ImGui::SameLine();
                Utils::renderColoredSQL(log_line.substr(11), true);
            } else if (log_line.find("Error") != std::string::npos) {
                ImGui::TextColored(GUI_RED, "%s", log_line.c_str());
            } else {
                ImGui::TextUnformatted(log_line.c_str());
            }
        }
        ImGui::EndChild();

        // TABLE RESULTS
        ImGui::Separator();
        ImGui::Text("Table Results:");
        if (!gui_headers.empty() && ImGui::BeginTable("ResultsTable", (int)gui_headers.size(), ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {
            for (const auto& h : gui_headers) ImGui::TableSetupColumn(h.c_str());
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

        // DATABASE SCHEMA layout
        ImGui::NextColumn();
        ImGui::BeginChild("SchemaBrowser");
        ImGui::TextColored(ImVec4(0.2f, 0.3f, 0.55f, 1.0f), "Database schema");
        ImGui::Separator();

        auto tableNames = db.getTableNames();
        for (const auto& tName : tableNames) {
            if (ImGui::CollapsingHeader(tName.c_str())) {
                try {
                    Table& t = db.getTable(tName);
                    for (const auto& col : t.getColumns()) {
                        ImGui::BulletText("%s (%s)", col.getName().c_str(), col.getTypeString().c_str());
                    }
                } catch (...) {}
            }
        }

        // EXIT BUTTON
        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 60);
        ImGui::Separator();
        if (ImGui::Button("Exit Program", ImVec2(-FLT_MIN, 40))) {
            glfwSetWindowShouldClose(window, true);
        }

        ImGui::EndChild();
        ImGui::End();

        ImGui::Render();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); glClear(GL_COLOR_BUFFER_BIT);
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