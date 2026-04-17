/**
* @file InsertStmt.hpp
 * @brief Implementation of the INSERT INTO statement execution logic.
 */
#include "Statement.hpp"
#include <vector>
#pragma once

/**
* @class InsertStmt
 * @brief Class that represents and manages the execution of a SQL INSERT INTO statement.
 */
class InsertStmt : public Statement {
private:
    std::string tableName;
    // for inserting many rows at once
    std::vector<std::vector<std::string>> allRows;
public:
    /**
     * @brief Constructs a new InsertStmt object for inserting many rows.
     * @param name The name of the table to insert into.
     * @param allValuesList A vector of rows (each row being a vector of strings) to insert.
     */
    InsertStmt (std::string name, std::vector<std::vector<std::string>> allValuesList) {
        this->tableName = std::move(name);
        this->allRows = std::move(allValuesList);
    }

    /**
     * @brief Constructs a new InsertStmt object for inserting one row.
     * @param name The name of the table to insert into.
     * @param singleRow A vector of strings representing the values of a single row.
     */
    InsertStmt(std::string name, std::vector<std::string> singleRow){
        this->tableName = std::move(name);
        this->allRows.push_back(std::move(singleRow));
    }

    /**
     * @brief Executes the INSERT INTO statement logic.
     * * The execution process follows:
     * 1. Get the target table from the database engine.
     * 2. Identify the target columns and their expected data types.
     * 3. Validate that the number of provided values matches the table's column count.
     * 4. Convert raw string values into the appropriate Cell types (INT, DOUBLE, BOOL, or TEXT).
     * 5. Insert the constructed row of Cells into the table storage.
     * 6. Catch and log any exceptions or success message
     */
    void execute() override;
    const std::string& getTableName() const { return tableName; }
    const std::vector<std::vector<std::string>>& getValues() const { return allRows; }
};