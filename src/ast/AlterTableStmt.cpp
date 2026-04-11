#include "ast/AlterTableStmt.hpp"
#include <iostream>
#include <map>
#include <vector>

extern std::map<std::string, std::vector<std::map<std::string, std::string>>> mockDatabase;

void AlterTableStmt::execute() {
    if (mockDatabase.find(tableName) == mockDatabase.end()) {
        std::cerr << "[ERROR] Cannot change the structure of the table: '" << tableName << "' because id does not exists" << std::endl;
        return;
    }

    // removing every column from certain row
    for (auto& row : mockDatabase[tableName]) {
        row.erase(columnToDelete);
    }

    std::cout << "[EXECUTOR] ALTER TABLE: from table" << tableName
              << " deleting columns: " << columnToDelete << std::endl;
}