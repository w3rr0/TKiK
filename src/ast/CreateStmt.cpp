#include "ast/CreateStmt.hpp"
#include <iostream>
#include "storage/Database.hpp"
#include "storage/Cell.hpp"
#include <stdexcept>

extern Database db;
extern std::vector<std::string> gui_log;

// helper method for mapping our data types from the stoarge
Cell::Type mapUserType(const std::string& type) {
    if (type == "INT" || type == "INTEGER") return Cell::INT;
    if (type == "DOUBLE" || type == "FLOAT" || type == "NUMERIC") return Cell::DOUBLE;
    if (type == "TEXT" || type == "STRING" || type == "VARCHAR") return Cell::TEXT;
    if (type == "BOOL" || type == "BOOLEAN") return Cell::BOOL;

    return Cell::NULL_TYPE;
}
void CreateStmt::execute() {
    try {
        db.createTable(tableName);

        Table& table = db.getTable(tableName);

        std::string msg = "CREATED TABLE " + tableName;
        gui_log.push_back(msg);

        // adding columns
        for (const auto& col : columns) {
            Cell::Type storageType = mapUserType(col.type);
            table.addColumn(col.name, storageType);
        }

    } catch (const std::exception& e) {
        std::string errMsg = e.what();
        gui_log.push_back("Error: " + errMsg);
    }
}