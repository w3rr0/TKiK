#include "ast/DeleteStmt.hpp"
#include <iostream>
#include "storage/Database.hpp"

extern Database db;
extern std::vector<std::string> gui_log;

void DeleteStmt::execute() {
    try {
        Table& table = db.getTable(tableName);
        int deleteCount = 0;

        for (size_t i = 0; i < table.getRowCount(); i++) {
            // skipping rows that are marked as deleted
            if (table.isDeleted(i)) continue;

            std::vector<Cell> row = table.getRow(i);

            // WHERE condition
            if (!where || where->evaluate(table, row)) {
                table.deleteRow(i);
                deleteCount++;
            }
        }
        std::string msg = "DELETED " + std::to_string(deleteCount) + " rows";
        std::cout << msg << std::endl;
        gui_log.push_back(msg);

    } catch (const std::exception& e) {
        std::string errMsg = e.what();
        gui_log.push_back("Error: " + errMsg);
    }
}