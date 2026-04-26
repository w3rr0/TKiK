#include <vector>
#include <string>
#include "storage/Database.hpp"

Database db("testDatabase");
std::vector<std::vector<std::string>> gui_results;
std::vector<std::string> gui_headers;
std::string gui_error = "";
std::vector<std::string> gui_log;

void executeSQL(const char* input) {}