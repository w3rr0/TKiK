/**
* @file AlterTableStmt.hpp
 * @brief Implementation of the ALTER TABLE statement execution logic.
 */
#include "Statement.hpp"
#include "storage/Cell.hpp"
#pragma once

/**
 * @class AlterTableStmt
 * @brief Class that represents and manages the execution of a SQL ALTER TABLE statement.
 */
class AlterTableStmt : public Statement {
public:
    /**
     * @brief Defines the type of schema modification to perform.
     */
    enum Action {
        ADD,
        DROP
    };
private:
    std::string tableName;
    std::string columnName;
    Cell::Type colType;
    Action action;
public:

    /**
     * @brief Constructor for the DROP COLUMN operation.
     * @param table The name of the table to modify.
     * @param col The name of the column to remove.
     * @param act The action type (DROP).
     */
    AlterTableStmt(std::string table, std::string col, Action act)
        : tableName(std::move(table)), columnName(std::move(col)), action(act), colType(Cell::NULL_TYPE) {}

    /**
     * @brief Constructor for the ADD COLUMN operation.
     * @param table The name of the table to modify.
     * @param col The name of the new column.
     * @param type The data type for the new column.
     * @param act The action type (ADD).
     */
    AlterTableStmt(std::string table, std::string col, Cell::Type type, Action act)
        : tableName(std::move(table)), columnName(std::move(col)), colType(type), action(act) {}

    /**
     * @brief Executes the ALTER TABLE statement logic.
     * * The execution process follows:
     * 1. Get the target table from the database engine.
     * 2. Identify the requested action (ADD or DROP).
     * 3. For ADD: Validate if the column name already exists to prevent duplicates.
     * 4. For ADD: Instruct the storage engine to append a new column of the specified type.
     * 5. For DROP: Locate the column index and remove it from the table structure.
     * 6. Log the successful modification message to the console and GUI log.
     * 7. Catch and log any exceptions or success message.
     */
    void execute() override;
};