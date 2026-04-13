#include "Statement.hpp"
#include <vector>
#pragma once

class InsertStmt : public Statement {
private:
    std::string tableName;
    // for inserting many rows at once
    std::vector<std::vector<std::string>> allRows;
public:
    // constructor for inserting many rows
    InsertStmt (std::string name, std::vector<std::vector<std::string>> allValuesList) {
        this->tableName = std::move(name);
        this->allRows = std::move(allValuesList);
    }

    // constructor for inserting one row
    InsertStmt(std::string name, std::vector<std::string> singleRow){
        this->tableName = std::move(name);
        this->allRows.push_back(std::move(singleRow));
    }

    void execute() override;
    const std::string& getTableName() const { return tableName; }
    const std::vector<std::vector<std::string>>& getValues() const { return allRows; }
};