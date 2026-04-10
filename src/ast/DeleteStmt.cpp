#include "ast/DeleteStmt.hpp"
#include <iostream>

void DeleteStmt::execute() {
    std::cout << "[EXECUTOR] DELETE FROM " << tableName;
    if (where) {
        std::cout << " | Condition: ";
        where->print();
    } else {
        std::cout << " | No condition";
    }
    std::cout << std::endl;
}