#include "ast/AlterTableStmt.hpp"
#include <iostream>
#include <storage/Database.hpp>
#include<storage/Table.hpp>
#include "parser.hpp"

extern Database db;
extern std::vector<std::string> gui_log;

void AlterTableStmt::execute() {
    try {
        Table& table = db.getTable(tableName);
        if (action == ADD) {
            table.addColumn(columnName, colType);

            std::string msg = "ALTER TABLE: column " + columnName + " added to the table " + table.getName();
            std::cout << msg << std::endl;
            gui_log.push_back(msg);
        } else if (action == DROP) {
            table.dropColumn(columnName);
            std::string msg = "ALTER TABLE: column " + columnName + " dropped from the table " + table.getName();
            std::cout << msg << std::endl;
            gui_log.push_back(msg);
        }
    } catch (const std::exception& e) {
        std::string err = e.what();
        gui_log.push_back(std::string("ALTER TABLE failed: " + err));
    }
}
