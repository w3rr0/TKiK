#include "Statement.hpp"
#pragma once

class DropTableStmt : public Statement {
private:
    std::string tableName;
public:
    explicit DropTableStmt (std::string name) {
        this->tableName = std::move(name);
    }

    void execute() override;
    const std::string& getTableName() const { return tableName; }
};
