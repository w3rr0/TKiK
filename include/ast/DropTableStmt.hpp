/**
* @file DropTableStmt.hpp
 * @brief Implementation of the DROP TABLE statement execution logic.
 */
#include "Statement.hpp"
#include <string>
#pragma once

/**
* @class DropTableStmt
 * @brief Class that represents and manages the execution of a SQL DROP TABLE statement.
 */
class DropTableStmt : public Statement {
private:
    std::string tableName;
public:
    /**
     * @brief Constructs a new DropTableStmt object.
     * @param name The name of the table to drop.
     */
    explicit DropTableStmt (std::string name) {
        this->tableName = std::move(name);
    }

    /**
     * @brief Executes the DROP TABLE statement logic.
     * * The execution process follows:
     * 1. Call the database engine to remove the table by name.
     * 2. Generate and log an error message to the GUI log in case of failure or success info of the operation.
     */
    void execute() override;
    const std::string& getTableName() const { return tableName; }
};
