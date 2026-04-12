#include "ast/DropTableStmt.hpp"
#include <iostream>
#include "storage/Database.hpp"
#include <stdexcept>

extern Database db;

void DropTableStmt::execute() {
    try {
        // removes the table
        db.dropTable(tableName);
        std::cout << "DROP TABLE '" << tableName << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error. DROP TABLE failed " << e.what() << std::endl;
    }
}