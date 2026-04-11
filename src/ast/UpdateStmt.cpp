#include "ast/UpdateStmt.hpp"
#include <iostream>
#include <map>

extern std::map<std::string, std::vector<std::map<std::string, std::string>>> mockDatabase;

void UpdateStmt::execute() {
    if (mockDatabase.find(tableName) == mockDatabase.end()) return;

    int updatedCount = 0;
    for (auto& row : mockDatabase[tableName]) {
        if (!where || where->evaluate(row)) {
            row[colName] = newValue; // changing value in certain row
            updatedCount++;
        }
    }
    std::cout << "[EXECUTOR] Updated " << updatedCount << " rows in " << tableName << std::endl;
}