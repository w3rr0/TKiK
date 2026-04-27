#include "gui/Utils.hpp"
#include <regex>
#include "imgui.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <filesystem>
#include <sstream>
#include <regex>

#include "gui/Utils.hpp"

// method taht renders SQL text with syntax highlighting using Regex
void Utils::renderColoredSQL(const std::string& text, bool isGui) {
    // sql keywords
    static std::regex regKeywords("\\b(SELECT|FROM|WHERE|CREATE|TABLE|INSERT|INTO|VALUES|UPDATE|DELETE|SET|DROP|"
                                  "COLUMN|ALTER|ADD|LIMIT|OFFSET""|DISTINCT|IS|NOT|AND|OR|ORDER|BY|ASC|DESC|BETWEEN|"
                                  "LIKE|IN|COUNT|MIN|MAX|SUM|SHOW|TABLES)\\b", std::regex_constants::icase);
    // sql data types
    static std::regex regTypes("\\b(INT|INTEGER|STRING|TEXT|VARCHAR|BOOL|BOOLEAN|DOUBLE|FLOAT|NULL)\\b",
                                    std::regex_constants::icase);
    // numbers
    static std::regex regNumbers("\\b\\d+(\\.\\d+)?\\b");
    // strings
    static std::regex regStrings("('\\s*[^']*\\s*'|\\\"\\s*[^\\\"]*\\s*\\\")");
    // connectting words into a sentence
    static std::regex wordRegex("([\\w*]+|'[^']*'|\\\"[^\\\"]*\\\"|[,;\\(\\)=<>!\\.]+|[ ]+)");

    auto words_begin = std::sregex_iterator(text.begin(), text.end(), wordRegex);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::string part = i->str();
        ImVec4 color = GUI_WHITE;
        std::string cliColor = CLI_WHITE;

        // applying correct colors based on the token type
        if (std::regex_match(part, regKeywords)) { color = GUI_RED; cliColor = CLI_RED; }
        else if (std::regex_match(part, regTypes)) { color = GUI_PINK; cliColor = CLI_PINK; }
        else if (std::regex_match(part, regNumbers)) { color = GUI_YELLOW; cliColor = CLI_YELLOW; }
        else if (std::regex_match(part, regStrings)) { color = GUI_GREEN; cliColor = CLI_GREEN; }

        if (isGui) {
            if (part[0] == ' ') {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::CalcTextSize(part.c_str()).x);
            } else {
                ImGui::TextColored(color, "%s", part.c_str());
                ImGui::SameLine(0, 0); // keeps symbols like '(' or '*' with no space
            }
        } else {
            std::cout << cliColor << part << CLI_RESET;
        }
    }
    if (isGui) ImGui::NewLine(); else std::cout << std::flush;
}

// method for formatting and printing table results in console
void Utils::printTableCLI() {
    if (gui_headers.empty()) return;
    std::vector<size_t> colWidths(gui_headers.size());
    for (size_t i = 0; i < gui_headers.size(); ++i) {
        colWidths[i] = gui_headers[i].length();
        for (const auto& row : gui_results) {
            if (i < row.size()) {
                colWidths[i] = std::max(colWidths[i], row[i].length());
            }
        }
    }
    auto sep = [&]() {
        std::cout << "+";
        for (size_t w : colWidths) std::cout << std::string(w + 2, '-') << "+";
        std::cout << std::endl;
    };
    sep();
    std::cout << "|";
    for (size_t i = 0; i < gui_headers.size(); ++i) {
        std::cout << " " << std::left << std::setw(colWidths[i]) << gui_headers[i] << " |";
    }
    std::cout << std::endl;
    sep();
    for (const auto& row : gui_results) {
        std::cout << "|";
        for (size_t i = 0; i < row.size(); ++i) {
            std::cout << " " << std::left << std::setw(colWidths[i]) << row[i] << " |";
        }
        std::cout << std::endl;
    }
    sep();
}

// method with CLI loop
void Utils::runCLI() {
    std::cout << CLI_PINK << "SQL CLI Mode " << CLI_RESET;
    std::cout << CLI_WHITE << "(type 'exit' to quit)" << CLI_RESET << std::endl;

    char line[10024];
    // buffer for queries continued after 'ENTER'
    std::string queryEnter= "";

    std::cout << CLI_PINK << "Available tables: " << CLI_RESET;
    std::cout << CLI_WHITE << "(type 'show tables' to see)\n" << CLI_RESET;


    while (true) {
        // if is empty start of te line will be 'sqW' otherwise if we will be finishing the query in new line '->'
        if (queryEnter.empty()) {
            std::cout << CLI_WHITE << "sqW> " << CLI_RESET;
        } else {
            std::cout << CLI_WHITE << "  -> " << CLI_RESET;
        }

        if (!std::cin.getline(line, sizeof(line))) break;
        if (std::string(line) == "exit") break;
        if (strlen(line) == 0) continue;

        // adding line with space char to the buffer
        queryEnter += std::string(line) + " ";

        std::string lowerInput = line;
        for (auto & c : lowerInput) {
            c = (unsigned char)std::tolower(c);
        }

        if (lowerInput == "show tables") {
            auto names = db.getTableNames();
            if(names.empty()) {
                std::cout << "[No tables]";
            } else {
                for (const auto& n : names) {
                    std::cout << "[" << n << "] ";
                }
            }
            std::cout << "\n";
            // clearing buffer after finished query
            queryEnter = "";
            continue;
        }

        // if we find ';' in our buffer than the query should be ended after it and then optional errors may appear
        if (queryEnter.find(';') != std::string::npos) {
            std::cout << CLI_PINK << "Executing: " << CLI_RESET;
            renderColoredSQL(queryEnter, false);
            std::cout << std::endl;

            executeSQL(queryEnter.c_str());

            // log and table display
            if (!gui_log.empty()) {
                std::string last = gui_log.back();
                if (last.find("Error") != std::string::npos) {
                    std::cout << CLI_RED << last << CLI_RESET << std::endl;
                } else {
                    std::cout << CLI_GREEN << last << CLI_RESET << std::endl;
                }
            }
            if (!gui_results.empty()) {
                printTableCLI();
            }

            std::cout << std::endl;
            queryEnter = ""; // reseting buffer for new query
            gui_log.clear();
        }
    }
}