#pragma once
#include "Statement.hpp"
#include "WhereClause.hpp"
#include <memory>
#include "map"

class UpdateStmt : public Statement {
private:
    std::string tableName;
    std::unique_ptr<WhereClause> where;
    std::map<std::string, std::string> columnsToUpdate;


public:
    // Constructor for single column
    UpdateStmt(std::string name, std::string col, std::string val){
        this->tableName = std::move(name);
        columnsToUpdate[std::move(col)] = std::move(val);
        this->where = nullptr;
    }

    // Additional constructor for many columns to update
    UpdateStmt(std::string name, std::map<std::string, std::string> updates) {
        this->tableName = std::move(name);
        this->columnsToUpdate = std::move(updates);
        this->where = nullptr;
    }

    // setter
    void setWhere(std::unique_ptr<WhereClause> w) {
        where = std::move(w);
    }

    void execute() override;

    // getters
    const std::string& getTableName() const { return tableName; }
    const std::map<std::string, std::string>& getUpdates() const { return columnsToUpdate; }
    const WhereClause* getWhere() const { return where.get(); }
};