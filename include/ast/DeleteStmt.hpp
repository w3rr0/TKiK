/**
* @file DeleteStmt.hpp
 * @brief Implementation of the DELETE FROM statement execution logic.
 */

#include "Statement.hpp"
#include "WhereClause.hpp"
#pragma once

/**
 * @class DeleteStmt
 * @brief Class that represents and manages the execution of a SQL DELET FROM statement.
 */
class DeleteStmt : public Statement {
private:
    std::string tableName;
    std::unique_ptr<WhereClause> where;

public:
    /**
     * @brief Constructs a new DeleteStmt object.
     * @param name The name of the table to delete rows from.
     */
    explicit DeleteStmt(std::string name) {
        this->tableName = std::move(name);
        this->where =  nullptr;
    }

    // condition for parser
    /**
     * @brief Sets the filtering condition for the DELETE operation.
     * @param w A unique pointer to the constructed WhereClause.
     */
    void setWhere(std::unique_ptr<WhereClause> w) {
        where = std::move(w);
    }

    /**
     * @brief Executes the DELETE statement logic.
     * * The execution process follows:
     * 1. Get the target table from the database engine.
     * 2. Skip rows that are already marked as deleted.
     * 3. Evaluate the WHERE condition if tehre is any.
     * 4. Mark matching rows as deleted in the storage engine.
     * 5. Log the total number of deleted rows to the console and GUI log.
     * 6. Catch and log any exceptions
     */
    void execute() override;

    const std::string& getTableName() const { return tableName; }
    const WhereClause* getWhere() const { return where.get(); }
};
