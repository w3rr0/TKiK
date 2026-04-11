#include <iostream>
#include "parser.hpp"
#include "ast/Statement.hpp"
#include <map>
#include <vector>

std::map<std::string, std::vector<std::map<std::string, std::string>>> mockDatabase = {
    {"studenci", {
            {{"id", "1"}, {"nazwa", "Krzysztof"}, {"ocena", "5.0"}},
            {{"id", "2"}, {"nazwa", "Jan"}, {"ocena", "3.0"}},
            {{"id", "3"}, {"nazwa", "Anna"}, {"ocena", "4.5"}},
            {{"id", "4"}, {"nazwa", "Marek"}, {"ocena", "2.0"}},
            {{"id", "5"}, {"nazwa", "Zofia"}, {"ocena", "5.0"}}
    }}
};

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