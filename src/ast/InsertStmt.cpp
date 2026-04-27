#include "ast/InsertStmt.hpp"
#include <iostream>
#include "storage/Database.hpp"
#include "storage/Cell.hpp"
#include <stdexcept>

extern Database db;
extern std::vector<std::string> gui_log;

// helper method for removing '' and "" from new added values
static std::string cleanInput(std::string s) {
    if (s.size() >= 2 && ((s.front() == '\'' && s.back() == '\'') || (s.front() == '\"' && s.back() == '\"'))) {
        return s.substr(1, s.size() - 2);
    }
    return s;
}

void InsertStmt::execute() {
    try {
        Table& table = db.getTable(tableName);
        const auto& targetColumns = table.getColumns(); // all columns
        int insertedCount = 0;

        for (auto const& rowVal : allRows) {
            // checking if number of columns is correct
            if (rowVal.size() != targetColumns.size()) {
                std::cerr << "Error. Table '" << tableName << "' expects " << targetColumns.size()
                          << " values, but " << rowVal.size() << " provided." << std::endl;
                return;
            }

            // converting strings types to Cell type
            std::vector<Cell> rowToInsert;
            for (size_t i = 0; i < rowVal.size(); ++i) {
                std::string val = cleanInput(rowVal[i]);
                Cell::Type targetType = targetColumns[i].getType();

                std::string lowerVal = val;
                for (auto & c : lowerVal) {
                    c = (unsigned char)std::tolower(c);
                }

                if (lowerVal == "null") {
                    rowToInsert.emplace_back(Cell());
                    continue;
                }

                if (targetType == Cell::INT) {
                    rowToInsert.emplace_back(Cell(std::stoi(val)));
                } else if (targetType == Cell::DOUBLE) {
                    rowToInsert.emplace_back(Cell(std::stod(val)));
                } else if (targetType == Cell::BOOL) {
                    rowToInsert.emplace_back(Cell(val == "true" || val == "1"));
                } else {
                    rowToInsert.emplace_back(Cell(val)); // default text type
                }
            }

            table.insertRow(rowToInsert);
            insertedCount++;
        }
        std::string msg = "INSERTED " + std::to_string(insertedCount) + " new rows";
        gui_log.push_back(msg);

    } catch (const std::exception& e) {
        std::string errMsg = e.what();
        gui_log.push_back("Error: " + errMsg);
    }
}