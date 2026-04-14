#include "ast/SelectStmt.hpp"
#include <iostream>
#include <set>
#include <iomanip>
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
        const auto& allTableCols = table.getColumns();

        // mapping columns
        std::vector<size_t> projectionIdx;
        if (columns.size() == 1 && columns[0] == "*") {
            for (size_t i = 0; i < allTableCols.size(); ++i) {
                projectionIdx.push_back(i);
            }
        } else {
            for (const auto& name : columns) {
                projectionIdx.push_back(getColIdx(table, name));
            }
        }

        // vector with Cells
        std::vector<std::vector<Cell>> filteredRows;
        // FILTERS
        for (size_t i = 0; i < table.getRowCount(); ++i) {
            // skipping deleted rows
            if (table.isDeleted(i)) continue;

            std::vector<Cell> row = table.getRow(i);
            if (!where || where->evaluate(table, row)) {
                filteredRows.push_back(row);
            }
        }

        // ORDER BY
        if (!orderByColumn.empty()) {
            size_t sortIdx = getColIdx(table, orderByColumn);
            std::sort(filteredRows.begin(), filteredRows.end(), [&](const std::vector<Cell>& a, const std::vector<Cell>& b) {
                if (isAscending) return a[sortIdx] < b[sortIdx];
                return a[sortIdx] > b[sortIdx];
            });
        }

        // DISTINCT
        std::vector<std::vector<Cell>> finalRows;
        std::set<std::vector<Cell>> uniqueSet;

        for (const auto& fullRow : filteredRows) {
            std::vector<Cell> projectedRow;
            for (size_t idx : projectionIdx) {
                projectedRow.push_back(fullRow[idx]);
            }

            if (isDistinct) {
                if (uniqueSet.insert(projectedRow).second) {
                    finalRows.push_back(projectedRow);
                }
            } else {
                finalRows.push_back(projectedRow);
            }
        }

        // AGGREGATION
        if (aggType != Aggregate::NONE) {
            // COUNT(*)
            if (aggType == Aggregate::COUNT && aggColumn == "*") {
                std::cout << "[RESULT] COUNT(*): " << filteredRows.size() << std::endl;
                return;
            }
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
        std::cout << "\nFound " << filteredRows.size() << " rows:" << std::endl;

        // column names
        std::cout << "  ";
        for (size_t idx : projectionIdx) {
            std::cout << "| " << std::left << std::setw(12) << allTableCols[idx].getName();
        }
        std::cout << "|" << std::endl;

        // line
        std::cout << "  ";
        for (size_t i = 0; i < projectionIdx.size(); ++i) std::cout << "+-------------";
        std::cout << "+" << std::endl;

        // cells with correct data
        for (const auto& row : finalRows) {
            std::cout << "  ";
            for (const auto& cell : row) {
                std::cout << "| " << std::left << std::setw(12) << cell;
            }
            std::cout << "|" << std::endl;
        }
        std::cout << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error. SELECT failed: " << e.what() << std::endl;
    }
}