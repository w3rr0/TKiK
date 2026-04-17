/**
* @file WhereClause.hpp
 * @brief Header definition for the WhereClause base class and its derived condition types.
 */
#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <map>
#include <storage/Cell.hpp>
#include <storage/Table.hpp>

/**
 * @class WhereClause
 * @brief Abstract base class representing any condition in a WHERE clause.
 */
class WhereClause {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived condition objects.
     */
    virtual ~WhereClause() = default;

    /**
     * @brief Evaluates the condition for a specific row.
     * @param table Reference to the table being processed.
     * @param row The row data as a vector of Cells.
     * @return true if the row meets the condition, false otherwise.
     */
    virtual bool evaluate(const Table& table, const std::vector<Cell>& row) const = 0;

    /** @brief Prints the condition structure.*/
    virtual void print() const = 0;
};

/**
 * @class ComparisonCondition
 * @brief Handles simple comparisons (=, !=, <, >, >=, <=, LIKE).
 */
class ComparisonCondition : public WhereClause {
private:
    /** The column name to check. */
    std::string column;
    /** The operator string (e.g., ">="). */
    std::string op;
    /** The literal value to compare against. */
    std::string value;
public:
    /**
     * @brief Constructs a new ComparisonCondition object.
     * @param col The name of the target column.
     * @param oper The operator string.
     * @param val The value to compare with.
     */
    ComparisonCondition(std::string col, std::string oper, std::string val)
        : column(std::move(col)), op(std::move(oper)), value(std::move(val)) {}

    bool evaluate(const Table& table, const std::vector<Cell>& row) const override;
    void print() const override;

    const std::string& getColumn() const { return column; }
    const std::string& getOp() const { return op; }
    const std::string& getValue() const { return value; }
};

/**
 * @class LogicalCondition
 * @brief Handles logical operations (AND, OR) between two other conditions.
 */
class LogicalCondition : public WhereClause {
private:
    /** Left side condition. */
    std::unique_ptr<WhereClause> left;
    /** Logical operator (AND / OR). */
    std::string op;
    /** Right side condition. */
    std::unique_ptr<WhereClause> right;
public:
    /**
     * @brief Constructs a new LogicalCondition object.
     * @param l Unique pointer to the left condition.
     * @param oper Logical operator string.
     * @param r Unique pointer to the right condition.
     */
    LogicalCondition(std::unique_ptr<WhereClause> l, std::string oper, std::unique_ptr<WhereClause> r)
        : left(std::move(l)), op(std::move(oper)), right(std::move(r)) {}

    bool evaluate(const Table& table, const std::vector<Cell>& row) const override;

    void print() const override;
};

/**
 * @class BetweenCondition
 * @brief Handles the BETWEEN condition (col BETWEEN val1 AND val2).
 */
class BetweenCondition : public WhereClause {
private:
    /** The column name to check. */
    std::string column;
    /** The minimum boundary value. */
    std::string valMin;
    /** The maximum boundary value. */
    std::string valMax;
public:
    /**
     * @brief Constructs a new BetweenCondition object.
     * @param col The name of the target column.
     * @param min The minimum value of the range.
     * @param max The maximum value of the range.
     */
    BetweenCondition(std::string col, std::string min, std::string max)
        : column(std::move(col)), valMin(std::move(min)), valMax(std::move(max)) {}

    bool evaluate(const Table& table, const std::vector<Cell>& row) const override;
    void print() const override;
};

/**
 * @class InCondition
 * @brief Handles the IN condition (col IN (val1, val2, ...)).
 */
class InCondition : public WhereClause {
private:
    /** The column name to check. */
    std::string column;
    /** List of allowed values. */
    std::vector<std::string> values;
public:
    /**
     * @brief Constructs a new InCondition object.
     * @param col The name of the target column.
     * @param vals A vector of string values to match against.
     */
    InCondition(std::string col, std::vector<std::string> vals)
        : column(std::move(col)), values(std::move(vals)) {}

    bool evaluate(const Table& table, const std::vector<Cell>& row) const override;
    void print() const override;
};