#include "Statement.hpp"
#pragma once

class InsertStmt : public Statement {
private:
    std::string tableName;
    std::vector<std::string> values;
public:
    InsertStmt (std::string name, std::vector<std::string> valuesList) {
        this->tableName = std::move(name);
        this->values = std::move(valuesList);
    }

    void execute() override;
    const std::string& getTableName() const { return tableName; }
    const std::vector<std::string>& getValues() const { return values; }
};