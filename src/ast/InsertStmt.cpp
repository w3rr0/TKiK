#include "ast/InsertStmt.hpp"
#include <iostream>
#include "storage/Database.hpp"
#include "storage/Cell.hpp"
#include <stdexcept>

extern Database db;

// helper method for removing '' and "" from new added values
std::string cleanInput(std::string s) {
    if (s.size() >= 2 && ((s.front() == '\'' && s.back() == '\'') || (s.front() == '\"' && s.back() == '\"'))) {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

void InsertStmt::execute() {
    try {
        // getting the table from database
        Table& table = db.getTable(tableName);
        const auto& targetColumns = table.getColumns(); // all columns

        // checking if number of columns is correct
        if (values.size() != targetColumns.size()) {
            std::cerr << "Error. Table '" << tableName << "' expects " << targetColumns.size()
                      << " values, but " << values.size() << " provided." << std::endl;
            return;
        }

        // converting strings types to Cell type
        std::vector<Cell> rowToInsert;
        for (size_t i = 0; i < values.size(); ++i) {
            std::string val = cleanInput(values[i]);
            Cell::Type targetType = targetColumns[i].getType();

            if (targetType == Cell::INT) {
                rowToInsert.push_back(Cell(std::stoi(val))); // string -> int
            } else if (targetType == Cell::DOUBLE) {
                rowToInsert.push_back(Cell(std::stod(val))); // string -> double
            } else if (targetType == Cell::BOOL) {
                rowToInsert.push_back(Cell(val == "true" || val == "1")); // true -> 1, false -> 0
            } else {
                rowToInsert.push_back(Cell(val)); // default text type
            }
        }

        // inserting row to our engine
        table.insertRow(rowToInsert);
        std::cout << "INSERTED NEW ROW INTO " << tableName << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error. INSERT failed: " << e.what() << std::endl;
    }
}