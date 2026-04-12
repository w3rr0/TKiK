#include "ast/SelectStmt.hpp"
#include <iostream>
#include <set>
#include "storage/Database.hpp"

extern Database db;

// helper method
static size_t getColIdx(const Table& table, const std::string& colName) {
    const auto& cols = table.getColumns();
    for (size_t i = 0; i < cols.size(); ++i) {
        if (cols[i].getName() == colName) return i;
    }
    throw std::invalid_argument("Column not found: " + colName);
}

void SelectStmt::execute() {
    try {
        Table& table = db.getTable(tableName);

        // vector with Cells
        std::vector<std::vector<Cell>> filteredRows;

        // FILTERS
        for (size_t i = 0; i < table.getRowCount(); ++i) {
            std::vector<Cell> row = table.getRow(i);
            if (!where || where->evaluate(table, row)) {
                filteredRows.push_back(row);
            }
        }

        // DISTINCT
        if (isDistinct && !filteredRows.empty()) {
            std::set<std::vector<Cell>> uniqueSet;
            std::vector<std::vector<Cell>> distinctRows;
            for (const auto& row : filteredRows) {
                if (uniqueSet.insert(row).second) distinctRows.push_back(row);
            }
            filteredRows = std::move(distinctRows);
        }

        // AGGREGATION
        if (aggType != Aggregate::NONE) {
            size_t colIdx = getColIdx(table, aggColumn);

            if (aggType == Aggregate::COUNT) {
                std::cout << "[RESULT] COUNT: " << filteredRows.size() << std::endl;
            }
            else if (!filteredRows.empty()) {
                if (aggType == Aggregate::SUM) {
                    double sum = 0;
                    for (const auto& r : filteredRows) {
                        const Cell& c = r[colIdx];
                        // summing as int or double
                        if (c.getType() == Cell::INT) sum += c.as<int>();
                        else if (c.getType() == Cell::DOUBLE) sum += c.as<double>();
                    }
                    std::cout << "[RESULT] SUM(" << aggColumn << "): " << sum << std::endl;
                }
                else if (aggType == Aggregate::MIN || aggType == Aggregate::MAX) {
                    Cell result = filteredRows[0][colIdx]; // first Cell is both min and max at first

                    for (const auto& r : filteredRows) {
                        const Cell& current = r[colIdx];
                        if (aggType == Aggregate::MIN) {
                            if (current < result) result = current; // operator <
                        } else {
                            if (current > result) result = current; // operator >
                        }
                    }
                    std::string label = (aggType == Aggregate::MIN) ? "MIN" : "MAX";
                    std::cout << "[RESULT] " << label << "(" << aggColumn << "): " << result << std::endl;
                }
            }
            return; // end of execute()
        }

        // DISPLAY with << operator
        std::cout << "Found " << filteredRows.size() << " rows:" << std::endl;
        for (const auto& row : filteredRows) {
            std::cout << "  | ";
            for (const auto& cell : row) std::cout << cell << " | ";
            std::cout << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error. SELECT failed: " << e.what() << std::endl;
    }
}