#include "ast/CreateStmt.hpp"
#include <iostream>
#include "storage/Database.hpp"
#include "storage/Cell.hpp"
#include <stdexcept>

extern Database db;

// helper method for mapping our data types from the stoarge
Cell::Type mapUserType(const std::string& type) {
    if (type == "INT" || type == "INTEGER") return Cell::INT;
    if (type == "DOUBLE" || type == "FLOAT" || type == "REAL") return Cell::DOUBLE;
    if (type == "TEXT" || type == "STRING" || type == "VARCHAR") return Cell::TEXT;
    if (type == "BOOL" || type == "BOOLEAN") return Cell::BOOL;

    return Cell::NULL_TYPE;
}
void CreateStmt::execute() {
    try {
        // creating new table in our database
        db.createTable(tableName);

        // reference to our new table
        Table& table = db.getTable(tableName);

        std::cout << "CREATED TABLE " << tableName << std::endl;

        // adding columns
        for (const auto& col : columns) {
            Cell::Type storageType = mapUserType(col.type);
            table.addColumn(col.name, storageType);
        }

    } catch (const std::exception& e) {
        std::cerr << "Error. CREATE TABLE failed " << e.what() << std::endl;
    }
}