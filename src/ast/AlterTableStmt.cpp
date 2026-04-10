#include "ast/AlterTableStmt.hpp"
#include <iostream>

void AlterTableStmt::execute() {
    std::cout << "[EXECUTOR] ALTER TABLE: " << tableName << std::endl;
    std::cout << "  - Removing column: " << columnToDelete << std::endl;
}