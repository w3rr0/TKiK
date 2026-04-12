#include <iostream>
#include "parser.hpp"
#include "ast/Statement.hpp"
#include "storage/Database.hpp"

// database
Database db("database");

extern int yyparse();
extern Statement* root_statement;
extern void yyrestart(FILE*);

int main() {
    std::cout << "--- SQL AST Interpreter ---" << std::endl;
    while (true) {
        std::cout << "SQL> ";
        if (yyparse() == 0) {
            if (root_statement != nullptr) {
                root_statement->execute();
            } else {
                std::cout << "[DEBUG] root statement is null" << std::endl;
            }
        } else {
            std::cout << "[DEBUG] parser error" << std::endl;
            yyrestart(stdin);
        }
    }
    return 0;
}