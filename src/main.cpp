#include <iostream>
#include "parser.hpp"
#include "ast/Statement.hpp"

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
                std::cout << "[DEBUG] Parser skończył, ale root_statement jest NULL!" << std::endl;
            }
        } else {
            std::cout << "[DEBUG] Parser zwrócił błąd (yyparse != 0)." << std::endl;
            yyrestart(stdin);
        }
    }
    return 0;
}