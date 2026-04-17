/**
* @file UpdateStmt.hpp
 * @brief Implementation of the UPDATE statement execution logic.
 */
#pragma once
#include "Statement.hpp"
#include "WhereClause.hpp"
#include <memory>
#include "map"

/**
 * @class UpdateStmt
 * @brief Class that represents and manages the execution of a SQL UPDATE statement.
 */
class UpdateStmt : public Statement {
private:
    std::string tableName;
    std::unique_ptr<WhereClause> where;
    std::map<std::string, std::string> columnsToUpdate;


public:
    /**
     * @brief Constructor for a single column update.
     * @param name The name of the table.
     * @param col The column name to update.
     * @param val The new value as a string.
     */
    UpdateStmt(std::string name, std::string col, std::string val){
        this->tableName = std::move(name);
        columnsToUpdate[std::move(col)] = std::move(val);
        this->where = nullptr;
    }

    /**
     * @brief Constructor for updating multiple columns at once.
     * @param name The name of the table.
     * @param updates A map containing multiple column-value pairs.
     */
    UpdateStmt(std::string name, std::map<std::string, std::string> updates) {
        this->tableName = std::move(name);
        this->columnsToUpdate = std::move(updates);
        this->where = nullptr;
    }

    // setter
    void setWhere(std::unique_ptr<WhereClause> w) {
        where = std::move(w);
    }

    /**
     * @brief Executes the UPDATE statement logic.
     * * The execution process follows:
     * 1. Get the target table from the database engine.
     * 2. Evaluate the WHERE condition for the current row.
     * 3. Update certain rows according to the condition;
     * 4. Log the final row count to the console and GUI History Log.
     */
    void execute() override;

    // getters
    const std::string& getTableName() const { return tableName; }
    const std::map<std::string, std::string>& getUpdates() const { return columnsToUpdate; }
    const WhereClause* getWhere() const { return where.get(); }
};