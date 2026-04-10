#include "ast/CreateStmt.hpp"
#include <iostream>

void CreateStmt::execute() {
    std::cout << "[EXECUTOR] CREATE TABLE: " << tableName << std::endl;
    for(const auto& col : columns) {
        std::cout << "  - Adding column: " << col.name << " (Type: " << col.type << ")" << std::endl;
    }
}