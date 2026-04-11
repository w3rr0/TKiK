#include "ast/DropTableStmt.hpp"
#include <iostream>
#include <map>
#include <vector>

extern std::map<std::string, std::vector<std::map<std::string, std::string>>> mockDatabase;

void DropTableStmt::execute() {
    if (mockDatabase.erase(tableName)) {
        std::cout << "[EXECUTOR] DROP TABLE: Table '" << tableName << "' has been deleted from database" << std::endl;
    } else {
        std::cerr << "[ERROR] Can not delete table " << tableName << "' does not exist" << std::endl;
    }
}