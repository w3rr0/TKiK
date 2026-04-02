#include <iostream>
#include <cstdio>

// deklarujemy to, co wygeneruje Flex i Bison
int yyparse();
extern FILE* yyin;

int main() {
    std::cout << "--- SQL Interpreter ---" << std::endl;
    std::cout << "> ";

    // standardowe wejście
    yyin = stdin;

    // wywołanie parsera
    if (yyparse() == 0) {
        std::cout << "[Sukces] Kompilacja udana." << std::endl;
    } else {
        std::cout << "[Blad] Cos poszlo nie tak." << std::endl;
    }

    return 0;
}