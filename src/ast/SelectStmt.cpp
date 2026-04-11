#include "ast/SelectStmt.hpp"
#include <iostream>
#include <algorithm>
#include <map>
#include <set>

extern std::map<std::string, std::vector<std::map<std::string, std::string>>> mockDatabase;

void SelectStmt::execute() {
    // checking if database exists
    if (mockDatabase.find(tableName) == mockDatabase.end()) {
        std::cerr << "[ERROR] Table '" << tableName << "' does not exist in database" << std::endl;
        return;
    }

    auto& allRows = mockDatabase[tableName];
    std::vector<std::map<std::string, std::string>> filteredRows;

    // filters
    for (const auto& row : allRows) {
        if (!where || where->evaluate(row)) {
            filteredRows.push_back(row);
        }
    }

    // DISTINCT
    if (isDistinct && !filteredRows.empty()) {
        std::set<std::map<std::string, std::string>> uniqueSet;
        std::vector<std::map<std::string, std::string>> distinctRows;

        for (const auto& row : filteredRows) {
            if (uniqueSet.find(row) == uniqueSet.end()) {
                uniqueSet.insert(row);
                distinctRows.push_back(row);
            }
        }
        filteredRows = std::move(distinctRows);
    }

    // ORDER BY
    if (!orderByColumn.empty() && !filteredRows.empty()) {
        std::sort(filteredRows.begin(), filteredRows.end(), [&](const auto& a, const auto& b) {
            if (isAscending) return a.at(orderByColumn) < b.at(orderByColumn);
            return a.at(orderByColumn) > b.at(orderByColumn);
        });
    }

    // AGGREGATION
    if (aggType != Aggregate::NONE) {
        if (aggType == Aggregate::COUNT) {
            std::cout << "[RESULT] COUNT: " << filteredRows.size() << std::endl;
        }
        else if (aggType == Aggregate::SUM && !filteredRows.empty()) {
            double sum = 0;
            for(const auto& r : filteredRows) {
                try { sum += std::stod(r.at(aggColumn)); } catch(...) {}
            }
            std::cout << "[RESULT] SUM(" << aggColumn << "): " << sum << std::endl;
        }
        else if ((aggType == Aggregate::MIN || aggType == Aggregate::MAX) && !filteredRows.empty()) {
            double result = 0;
            bool firstFound = false;

            for (const auto& r : filteredRows) {
                try {
                    double currentVal = std::stod(r.at(aggColumn));
                    if (!firstFound) {
                        result = currentVal;
                        firstFound = true;
                    } else {
                        if (aggType == Aggregate::MIN) {
                            if (currentVal < result) result = currentVal;
                        } else {
                            if (currentVal > result) result = currentVal;
                        }
                    }
                } catch(...) { continue; }
            }

            if (firstFound) {
                std::string label = (aggType == Aggregate::MIN) ? "MIN" : "MAX";
                std::cout << "[RESULT] " << label << "(" << aggColumn << "): " << result << std::endl;
            } else {
                std::cout << "[RESULT] " << (aggType == Aggregate::MIN ? "MIN" : "MAX") << ": NULL (no numeric data)" << std::endl;
            }
        }
        // Wyjście z execute(), jeśli robiliśmy agregację
        return;
    }
    // results
    std::cout << "[RESULT] Found " << filteredRows.size() << " rows:" << std::endl;
    for (const auto& row : filteredRows) {
        std::cout << "  | ";
        if (columns.size() == 1 && columns[0] == "*") {
            for(auto const& [key, val] : row) std::cout << key << ": " << val << " | ";
        } else {
            for(const auto& col : columns) {
                if(row.count(col)) std::cout << col << ": " << row.at(col) << " | ";
            }
        }
        std::cout << std::endl;
    }
}