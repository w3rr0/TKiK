#include "Statement.hpp"
#include "storage/Cell.hpp"
#pragma once

class AlterTableStmt : public Statement {
public:
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

    // constructor for DROP COLUMN
    AlterTableStmt(std::string table, std::string col, Action act)
        : tableName(std::move(table)), columnName(std::move(col)), action(act), colType(Cell::NULL_TYPE) {}

    // constructor for ADD COLUMN
    AlterTableStmt(std::string table, std::string col, Cell::Type type, Action act)
        : tableName(std::move(table)), columnName(std::move(col)), colType(type), action(act) {}

    void execute() override;
};