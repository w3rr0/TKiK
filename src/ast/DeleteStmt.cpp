#include "ast/DeleteStmt.hpp"
#include <iostream>
#include <algorithm>
#include <map>

extern std::map<std::string, std::vector<std::map<std::string, std::string>>> mockDatabase;

void DeleteStmt::execute() {
    if (mockDatabase.find(tableName) == mockDatabase.end()) return;

    auto& rows = mockDatabase[tableName];
    size_t initialSize = rows.size();

    rows.erase(std::remove_if(rows.begin(), rows.end(), [&](const auto& row) {
        return !where || where->evaluate(row);
    }), rows.end());

    std::cout << "[EXECUTOR] Deleted " << (initialSize - rows.size()) << " rows from " << tableName << std::endl;
}