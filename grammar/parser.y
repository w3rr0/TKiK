%{
#include <iostream>
#include <cstdio>
#include <string>

int yylex();
void yyerror(const char *s);
%}

// co mogą przechowywać tokeny (liczbę lub tekst)
%union {
    int num;
    char* str;
}

// wszystkie tokeny, które skaner będzie wysyłał i typy opcjonalne w <>
%token SELECT FROM WHERE CREATE TABLE INSERT INTO VALUES UPDATE DELETE SET
%token TYPE_INT TYPE_STRING TYPE_BOOL TYPE_DOUBLE TYPE_FLOAT
%token ORDER BY ASC DESC
%token IN LIKE BETWEEN
%token DISTINCT COUNT SUM MIN MAX
%token STAR EQ GT GE LE LT AND OR
%token LPAREN RPAREN COMMA SEMICOLON
%token <num> NUMBER
%token <str> ID STR

// Definicja priorytetów dla operatorów logicznych (zapobiega konfliktom)
%left OR
%left AND

%%
// 'cel:' + 'składnik1' + 'składnik2' + ';'
program:
    instrukcja SEMICOLON
    ;

// MOŻLIWE INSTRUKCJE
instrukcja:
    select_stmt
    | create_stmt
    | insert_stmt
    | update_stmt
    | delete_stmt
    ;

// SELECT z DISTINCT i ORDER BY
select_stmt:
    SELECT opt_distinct columns FROM ID where_clause opt_order_by {
        std::cout << "Akcja: SELECT z tabeli " << $5 << std::endl;
    }
    ;

opt_distinct:
    /* puste */
    | DISTINCT { std::cout << "  Opcja: DISTINCT" << std::endl; }
    ;

where_clause:
	/* puste  */
    | WHERE condition
    ;

// przyklady warunkow w WHERE
condition:
    ID EQ value         { std::cout << "  Warunek: " << $1 << " = value" << std::endl; }
    | ID GT value       { std::cout << "  Warunek: " << $1 << " > value" << std::endl; }
    | ID LT value       { std::cout << "  Warunek: " << $1 << " < value" << std::endl; }
    | ID GE value       { std::cout << "  Warunek: " << $1 << " >= value" << std::endl; }
    | ID LE value       { std::cout << "  Warunek: " << $1 << " <= value" << std::endl; }

    | ID LIKE STR       { std::cout << "  Warunek: " << $1 << " matches pattern " << $3 << std::endl; }
    | ID IN LPAREN value_list RPAREN { std::cout << "  Warunek: " << $1 << " in list" << std::endl; }
    | STR IN LPAREN value_list RPAREN { std::cout << "  Warunek: staly napis w liscie." << std::endl; }
    | ID BETWEEN value AND value {
            std::cout << "  Warunek: " << $1 << " pomiedzy wartosciami" << std::endl;
        }

    | condition AND condition { std::cout << "  Laczenie: AND" << std::endl; }
    | condition OR condition  { std::cout << "  Laczenie: OR" << std::endl; }
    | LPAREN condition RPAREN
    ;


id_list:
    ID                  { std::cout << "Kolumna: " << $1 << std::endl; }
    | id_list COMMA ID  { std::cout << "Kolumna: " << $3 << std::endl; }
    ;

// CREATE TABLE
create_stmt:
    CREATE TABLE ID LPAREN column_defs RPAREN {
        std::cout << "Tworze tabele: " << $3 << std::endl;
    }
    ;

// KOLUMNY
column_defs:
    column_def
    | column_defs COMMA column_def
    ;

// $1, $3 -> zmienne pseudo-identyfikacyjne w Bisonie
column_def:
    ID data_type {
        std::cout << "  Dodaje kolumne: " << $1 << std::endl;
    }
    ;

// TYPY
data_type:
    TYPE_INT | TYPE_STRING | TYPE_BOOL | TYPE_DOUBLE | TYPE_FLOAT
    ;

// INSERT INTO
insert_stmt:
    INSERT INTO ID VALUES LPAREN value_list RPAREN {
    		// to wykona sie, gdy parser uzna, ze dany fragment pasuje do wzorca
        std::cout << "Wstawiam dane do tabeli: " << $3 << std::endl;
    }
    ;

// DELETE
delete_stmt:
    DELETE FROM ID where_clause {
        std::cout << "Usuwanie z tabeli " << $3 << std::endl;
    }
    ;

// UPDATE
update_stmt:
    UPDATE ID SET ID EQ value where_clause {
        std::cout << "Aktualizacja tabeli " << $2 << ", kolumna " << $4 << std::endl;
    }
    ;

opt_order_by:
    /* puste */
    | ORDER BY ID opt_asc_desc { std::cout << "  Sortowanie po: " << $3 << std::endl; }
    ;

opt_asc_desc:
    /* puste */
    | ASC
    | DESC
    ;

// Agregacje w kolumnach
columns:
    STAR                { std::cout << "Wybrano wszystkie kolumny (*)" << std::endl; }
    | id_list           { std::cout << "Zakonczono wczytywanie listy kolumn." << std::endl; }
    | aggregate_func
    ;

aggregate_func:
    COUNT LPAREN STAR RPAREN { std::cout << "  Agregacja: COUNT(*)" << std::endl; }
    | SUM LPAREN ID RPAREN   { std::cout << "  Agregacja: SUM(" << $3 << ")" << std::endl; }
    | MIN LPAREN ID RPAREN   { std::cout << "  Agregacja: MIN(" << $3 << ")" << std::endl; }
    | MAX LPAREN ID RPAREN   { std::cout << "  Agregacja: MAX(" << $3 << ")" << std::endl; }
    ;

value_list:
    value
    | value_list COMMA value
    ;

value:
    NUMBER | STR | ID
    ;

%%

// funkcja, która zostanie wywołana automatycznie,
// jeśli użytkownik wpisze coś, co nie pasuje do gramatyki
void yyerror(const char *s) {
    std::cerr << "Blad skladu: " << s << std::endl;
}