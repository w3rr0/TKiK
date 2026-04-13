#include "ast/AlterTableStmt.hpp"
#include <iostream>
#include <storage/Database.hpp>
#include<storage/Table.hpp>
#include "parser.hpp"

extern Database db;

void AlterTableStmt::execute() {
    try {
        Table& table = db.getTable(tableName);
        if (action == ADD) {
            table.addColumn(columnName, colType);
            std::cout << "ALTER TABLE: column " << columnName << " added to the table " << table.getName() << std::endl;
        } else if (action == DROP) {
            table.dropColumn(columnName);
            std::cout << "ALTER TABLE: column " << columnName << " dropped from the table " << table.getName() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Error. ALTER TABLE failed " << e.what() << std::endl;
    }
}
