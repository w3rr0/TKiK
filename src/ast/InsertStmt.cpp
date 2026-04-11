#include "ast/InsertStmt.hpp"
#include <iostream>
#include <map>

extern std::map<std::string, std::vector<std::map<std::string, std::string>>> mockDatabase;

void InsertStmt::execute() {
    if (mockDatabase.find(tableName) == mockDatabase.end()) {
        std::cerr << "[ERROR] Table '" << tableName << "' does not exist" << std::endl;
        return;
    }

    // creating new row for now only for the mock database later to extend...
    std::map<std::string, std::string> newRow;
    if (values.size() >= 3) {
        newRow["id"] = values[0];
        newRow["nazwa"] = values[1];
        newRow["ocena"] = values[2];

        mockDatabase[tableName].push_back(newRow);
        std::cout << "[EXECUTOR] New rows added to database" << tableName << std::endl;
    } else {
        std::cerr << "[ERROR] invalid arguments" << std::endl;
    }
}