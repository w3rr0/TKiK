#include "ast/WhereClause.hpp"
#include <iostream>

void printWhere(const WhereClause* where) {
    if (where) {
        std::cout << "  WHERE ";
        where->print();
        std::cout << std::endl;
    }
}