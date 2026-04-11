#include "ast/CreateStmt.hpp"
#include <iostream>
#include <map>
#include <vector>

extern std::map<std::string, std::vector<std::map<std::string, std::string>>> mockDatabase;

void CreateStmt::execute() {
    if (mockDatabase.count(tableName)) {
        std::cerr << "[ERROR] table '" << tableName << "' already exists" << std::endl;
        return;
    }

    mockDatabase[tableName] = std::vector<std::map<std::string, std::string>>();

    std::cout << "[EXECUTOR] Created new table " << tableName << std::endl;
    for(const auto& col : columns) {
        std::cout << "  - Column: " << col.name << " (" << col.type << ")" << std::endl;
    }
}