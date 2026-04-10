#pragma once
#include "Statement.hpp"
#include "WhereClause.hpp"
#include <memory>

class UpdateStmt : public Statement {
private:
    std::string tableName;
    std::string colName;
    std::string newValue;
    std::unique_ptr<WhereClause> where;

public:
    UpdateStmt (std::string name, std::string col, std::string val) {
        this->tableName = std::move(name);
        this->colName = std::move(col);
        this->newValue = std::move(val);
        this->where = nullptr;
    }

    void setWhere(std::unique_ptr<WhereClause> w) {
        where = std::move(w);
    }

    void execute() override;

    const std::string& getTableName() const { return tableName; }
    const std::string& getColName() const { return colName; }
    const std::string& getNewValue() const { return newValue; }
    const WhereClause* getWhere() const { return where.get(); }
};