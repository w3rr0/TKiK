#include "ast/DropTableStmt.hpp"
#include <iostream>
#include "storage/Database.hpp"
#include <stdexcept>

extern Database db;
extern std::vector<std::string> gui_log;

void DropTableStmt::execute() {
    try {
        // removes the table
        db.dropTable(tableName);

        std::string msg = "DROP TABLE " + tableName;
        gui_log.push_back(msg);
    } catch (const std::exception& e) {
        std::string errMsg = "Error while executing DROP TABLE " + tableName;
        gui_log.push_back(errMsg);
    }
}