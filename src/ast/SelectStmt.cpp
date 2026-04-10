#include "ast/SelectStmt.hpp"
#include <iostream>

void SelectStmt::execute() {
    std::cout << "[EXECUTOR] SELECT: " << tableName << std::endl;

    // type of aggregation
    if (getAggType() != Aggregate::NONE) {
        std::string aggName;
        switch (getAggType()) {
            case Aggregate::COUNT: aggName = "COUNT"; break;
            case Aggregate::SUM:   aggName = "SUM"; break;
            case Aggregate::MIN:   aggName = "MIN"; break;
            case Aggregate::MAX:   aggName = "MAX"; break;
            default: aggName = "UNKNOWN";
        }
        std::cout << "  - Aggregation: " << aggName << "(" << getAggColumn() << ")" << std::endl;
    } else {
        std::cout << "  - Columns: ";
        for(const auto& col : columns) std::cout << col << " ";
        std::cout << std::endl;
    }

    std::cout << "  - DISTINCT: " << (isDistinct ? "YES" : "NO") << std::endl;

    // WHERE condition
    if (where) {
        std::cout << "  - Condition: ";
        where->print(); // Polimorficzne wyświetlanie
        std::cout << std::endl;
    }

    // SORTING
    if (!getOrderBy().empty()) {
        std::cout << "  - SORTING BY: " << getOrderBy() << (asc() ? " (ASC)" : " (DESC)") << std::endl;
    }
}