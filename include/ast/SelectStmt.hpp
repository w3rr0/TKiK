#pragma once
#include "Statement.hpp"
#include "WhereClause.hpp"
#include <vector>
#include <string>
#include <memory>

class SelectStmt : public Statement {
public:
    enum class Aggregate { NONE, COUNT, SUM, MIN, MAX };
private:
    std::string tableName;
    std::vector<std::string> columns;
    bool isDistinct;
    Aggregate aggType;
    std::string aggColumn = "";
    std::string orderByColumn = "";
    bool isAscending = true;
    std::unique_ptr<WhereClause> where;

public:
    SelectStmt(std::string table, std::vector<std::string> cols, bool distinct = false) {
        this->tableName = std::move(table);
        this->columns = std::move(cols);
        this->isDistinct = distinct;
    }

    // setters
    void setAggregate(Aggregate type, std::string col) { aggType = type; aggColumn = std::move(col); }
    void setOrder(std::string col, bool asc) { orderByColumn = std::move(col); isAscending = asc; }
    void setWhere(std::unique_ptr<WhereClause> w) { where = std::move(w); }

    void execute() override;
};