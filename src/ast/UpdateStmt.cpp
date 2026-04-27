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
    std::string lowerVal = clean;

    for (auto & c : lowerVal) {
        c = (unsigned char)std::tolower(c);
    }

    if (lowerVal == "null") {
        return Cell();
    }

    if (type == Cell::INT) return Cell(std::stoi(clean));
    if (type == Cell::DOUBLE) return Cell(std::stod(clean));
    if (type == Cell::BOOL) return Cell(clean == "true" || clean == "1");
    return Cell(clean); // TEXT
}

void UpdateStmt::execute() {
    try {
        Table& table = db.getTable(tableName);
        int updateCount = 0;

        std::vector<std::pair<std::string, Cell>> preparedUpdates;
        for (auto const& [colName, newVal] : columnsToUpdate) {
            Cell::Type targetType = table.getColumn(colName).getType();
            preparedUpdates.push_back({colName, createCellUpdate(newVal, targetType)});
        }
        for (size_t i = 0; i < table.getRowCount(); i++) {
            if (table.isDeleted(i)) continue;

            std::vector<Cell> row = table.getRow(i);

            if (!where || where->evaluate(table, row)) {
                for (auto const& [colName, newValue] : preparedUpdates) {
                    table.updateCell(i, colName, newValue);
                }
                updateCount++;
            }
        }

        std::string msg = "UPDATED " + std::to_string(updateCount) + " rows";
        gui_log.push_back(msg);

    } catch (const std::exception& e) {
        std::string errMsg = e.what();
        gui_log.push_back("Error: " + errMsg);
    }
}