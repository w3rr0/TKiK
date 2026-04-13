#include "ast/DeleteStmt.hpp"
#include <iostream>
#include "storage/Database.hpp"

extern Database db;

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
        std::cout << "DELETED " << deleteCount << " rows:" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error. DELETE failed " << e.what() << std::endl;
    }
}