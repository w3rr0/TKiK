#include <iostream>

#include "Statement.hpp"
#include "WhereClause.hpp"
#pragma once

class DeleteStmt : public Statement {
private:
    std::string tableName;
    std::unique_ptr<WhereClause> where;

public:
    explicit DeleteStmt(std::string name) {
        this->tableName = std::move(name);
        this->where =  nullptr;
    }

    // consition for parser
    void setWhere(std::unique_ptr<WhereClause> w) {
        where = std::move(w);
    }

    void execute() override;

    const std::string& getTableName() const { return tableName; }
    const WhereClause* getWhere() const { return where.get(); }
};
