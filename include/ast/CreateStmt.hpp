#include "Statement.hpp"
#include "ColumnDef.hpp"
#include <vector>
#pragma once

class CreateStmt : public Statement {
private:
    std::string tableName;
    std::vector<ColumnDef> columns;
public:
    CreateStmt(std::string name, std::vector<ColumnDef> cols) {
        this->tableName = std::move(name);
        this->columns = std::move(cols);
    }

    void execute() override;
    const std::string& getTableName() const { return tableName; }
    const std::vector<ColumnDef>& getColumns() const { return columns; }
};