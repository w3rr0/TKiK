#include "ast/UpdateStmt.hpp"
#include <iostream>

void UpdateStmt::execute() {
    std::cout << "[EXECUTOR] UPDATE TABLE: " << tableName << std::endl;
    std::cout << "  - Change in column: " << colName << " Set value: " << newValue << std::endl;

    if (where) {
        std::cout << "  - Condition: ";
        where->print();
        std::cout << std::endl;
    } else {
        std::cout << "  - Condition: NONE (Updating all rows)" << std::endl;
    }
}