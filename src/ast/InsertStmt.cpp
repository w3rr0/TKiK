#include "ast/InsertStmt.hpp"
#include <iostream>

void InsertStmt::execute() {
    std::cout << "[EXECUTOR] INSERT INTO: " << tableName << std::endl;
    std::cout << "  - Values: ";
    for(const auto& v : values) std::cout << v << ", ";
    std::cout << std::endl;
}