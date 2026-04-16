#ifndef UTILS_HPP
#define UTILS_HPP

#include "imgui.h"
#include <vector>
#include <string>
#include "storage/Database.hpp"

// colors in CLI for syntax highlighting
#define CLI_RED     "\033[31m"
#define CLI_PINK    "\033[35m"
#define CLI_YELLOW  "\033[33m"
#define CLI_GREEN   "\033[32m"
#define CLI_WHITE   "\033[37m"
#define CLI_RESET   "\033[0m"

// colors in GUI for syntax highlighting
const ImVec4 GUI_RED    = ImVec4(1.0f, 0.3f, 0.3f, 1.0f);
const ImVec4 GUI_PINK   = ImVec4(1.0f, 0.4f, 0.7f, 1.0f);
const ImVec4 GUI_YELLOW = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
const ImVec4 GUI_GREEN  = ImVec4(0.3f, 1.0f, 0.3f, 1.0f);
const ImVec4 GUI_WHITE  = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

// global declarations
extern Database db;
extern std::vector<std::vector<std::string>> gui_results;
extern std::vector<std::string> gui_headers;
extern std::string gui_error;
extern std::vector<std::string> gui_log;

// declaration of executeSQL so Utils can see it
void executeSQL(const char* input);

class Utils {
public:
    static void renderColoredSQL(const std::string& text, bool isGui);
    static void printTableCLI();
    static void runCLI();
};

#endif