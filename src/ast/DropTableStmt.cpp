#include "ast/DropTableStmt.hpp"
#include <iostream>

void DropTableStmt::execute() {
    std::cout << "[EXECUTOR] DROP TABLE: " << tableName << std::endl;
}