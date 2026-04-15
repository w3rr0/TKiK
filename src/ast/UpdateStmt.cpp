#include "ast/UpdateStmt.hpp"
#include <iostream>
#include <vector>
#include <storage/Database.hpp>

extern Database db;
extern std::vector<std::string> gui_log;

// method for cleaning '' / ""
static std::string clearQuotes(std::string s) {
    if (s.size() >= 2 && ((s.front() == '\'' && s.back() == '\'') || (s.front() == '\"' && s.back() == '\"'))) {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

// helper method that creates a Cell object with the correct type
Cell createCellUpdate(const std::string& val, Cell::Type type) {
    std::string clean = clearQuotes(val);
    if (type == Cell::INT) return Cell(std::stoi(clean));
    if (type == Cell::DOUBLE) return Cell(std::stod(clean));
    if (type == Cell::BOOL) return Cell(clean == "true" || clean == "1");
    return Cell(clean); // TEXT
}

void UpdateStmt::execute() {
    try {
        Table& table = db.getTable(tableName);
        int updateCount = 0;

        for (size_t i = 0; i < table.getRowCount(); i++) {
            // we skip the row that is marked as deleted
            if (table.isDeleted(i)) continue;

            std::vector<Cell> row = table.getRow(i);

            // checking for WHERE condition
            if (!where || where->evaluate(table, row)) {
                for (auto [colName, newVal] : columnsToUpdate) {
                    Cell::Type newType = table.getColumn(colName).getType();
                    Cell newValue = createCellUpdate(newVal, newType);

                    table.updateCell(i, colName, newValue);
                }
                updateCount++;
            }
        }
        std::string msg = "UPDATED " + std::to_string(updateCount) + " rows";
        std::cout << msg << std::endl;
        gui_log.push_back(msg);

    } catch (const std::exception& e) {
        std::string errMsg = e.what();
        gui_log.push_back("Error: " + errMsg);
    }
}