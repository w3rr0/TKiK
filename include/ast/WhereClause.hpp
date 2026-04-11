#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <map>

// All categories of WHERE conditions
class WhereClause {
public:
    virtual ~WhereClause() = default;

    // called by executor for every row
    // now always true, then will be Cell / Row
    virtual bool evaluate(const std::map<std::string, std::string>& row) const = 0;

    virtual void print() const = 0;
};

// Easy Condition -> operators >, <, =, <=, >=, LIKE
class ComparisonCondition : public WhereClause {
private:
    std::string column;
    std::string op;
    std::string value;
public:
    ComparisonCondition(std::string col, std::string oper, std::string val)
        : column(std::move(col)), op(std::move(oper)), value(std::move(val)) {}

    bool evaluate(const std::map<std::string, std::string>& row) const override;
    void print() const override;

    const std::string& getColumn() const { return column; }
    const std::string& getOp() const { return op; }
    const std::string& getValue() const { return value; }
};

// Logical Condition -> AND / OR
class LogicalCondition : public WhereClause {
private:
    std::unique_ptr<WhereClause> left;
    std::string op; // AND / OR
    std::unique_ptr<WhereClause> right;
public:
    LogicalCondition(std::unique_ptr<WhereClause> l, std::string oper, std::unique_ptr<WhereClause> r)
        : left(std::move(l)), op(std::move(oper)), right(std::move(r)) {}

    bool evaluate(const std::map<std::string, std::string>& row) const override;

    void print() const override;
};

// Condition col BETWEN val1 AND val2
class BetweenCondition : public WhereClause {
private:
    std::string column;
    std::string valMin;
    std::string valMax;
public:
    BetweenCondition(std::string col, std::string min, std::string max)
        : column(std::move(col)), valMin(std::move(min)), valMax(std::move(max)) {}

    bool evaluate(const std::map<std::string, std::string>& row) const override;
    void print() const override;
};

// List Condition -> IN
class InCondition : public WhereClause {
private:
    std::string column;
    std::vector<std::string> values;
public:
    InCondition(std::string col, std::vector<std::string> vals)
        : column(std::move(col)), values(std::move(vals)) {}

    bool evaluate(const std::map<std::string, std::string>& row) const override;
    void print() const override;
};