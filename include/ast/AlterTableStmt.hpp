#include "Statement.hpp"
#pragma once

class AlterTableStmt : public Statement {
private:
    std::string tableName;
    std::string columnToDelete;
public:
    AlterTableStmt(std::string table, std::string col) {
        this->tableName = std::move(table);
        this->columnToDelete = std::move(col);
    }

    void execute() override;
    const std::string& getTableName() const { return tableName; }
    const std::string& getColumnToDelete() const { return columnToDelete; }
};