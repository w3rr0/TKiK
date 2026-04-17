/**
* @file CreateStmt.hpp
 * @brief Implementation of the CREATE TABLE statement execution logic.
 */
#pragma
#include "Statement.hpp"
#include "ColumnDef.hpp"
#include <vector>
#pragma once

/**
 * @class CreateStmt
 * @brief Class that represents and manages the execution of a SQL CREATE TABLE statement.
 */
class CreateStmt : public Statement {
private:
    std::string tableName;
    std::vector<ColumnDef> columns;
public:
    /**
     * @brief Constructs a new CreateStmt object.
     * @param name The name of the table to create.
     * @param cols Vector of column names to select.
     */
    CreateStmt(std::string name, std::vector<ColumnDef> cols) {
        this->tableName = std::move(name);
        this->columns = std::move(cols);
    }

    /**
     * @brief Executes the CREATE TABLE statement logic.
     * * The execution process follows:
     * 1. adding new table in the database engine.
     * 2. Mapping user types ('INT', 'VARCHAR', ...) to storage types.
     * 3. Adding columns to the table structure.
     * 4. Handling potential errors and logging them or successfully created table.
     */
    void execute() override;
    const std::string& getTableName() const { return tableName; }
    const std::vector<ColumnDef>& getColumns() const { return columns; }
};