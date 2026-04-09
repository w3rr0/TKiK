%{
#include <iostream>
#include <cstdio>
#include <string>

int yylex();
void yyerror(const char *s);
%}

// what tokens can store (number or text)
%union {
    int num;
    char* str;
}

// all tokens that the scanner will send and optional types in <>
%token SELECT FROM WHERE CREATE TABLE INSERT INTO VALUES UPDATE DELETE SET DROP COLUMN ALTER

%token TYPE_INT TYPE_STRING TYPE_BOOL TYPE_DOUBLE TYPE_FLOAT TYPE_NULL

%token IN LIKE BETWEEN AND OR IS
%token DISTINCT COUNT SUM MIN MAX
%token ORDER BY ASC DESC

%token LPAREN RPAREN COMMA SEMICOLON
%token STAR EQ GT GE LE LT

%token <num> NUMBER
%token <str> ID STR

// define priorities for logical operators (prevents conflicts)
%left OR
%left AND

%%
// 'target:' + 'ingredient1' + 'ingredient2' + ';'
program:
    instrukcja SEMICOLON
    ;

// POSSIBLE INSTRUCTIONS
instrukcja:
    select_stmt
    | create_stmt
    | insert_stmt
    | update_stmt
    | delete_stmt
    | drop_table_stmt
    | alter_table_stmt
    ;

// SELECT with DISTINCT and ORDER BY
select_stmt:
    SELECT opt_distinct columns FROM ID where_clause opt_order_by {
        std::cout << "Action: SELECT from table " << $5 << std::endl;
    }
    ;

opt_distinct:
    /* blank */
    | DISTINCT { std::cout << "  Option: DISTINCT" << std::endl; }
    ;

where_clause:
/* blank  */
    | WHERE condition
    ;

// examples of conditions in WHERE
condition:
    ID EQ value         { std::cout << "  Condition: " << $1 << " = value" << std::endl; }
    | ID GT value       { std::cout << "  Condition: " << $1 << " > value" << std::endl; }
    | ID LT value       { std::cout << "  Condition: " << $1 << " < value" << std::endl; }
    | ID GE value       { std::cout << "  Condition: " << $1 << " >= value" << std::endl; }
    | ID LE value       { std::cout << "  Condition: " << $1 << " <= value" << std::endl; }
    | ID IS TYPE_NULL { std::cout << "  Condition: " << $1 << " is null" << std::endl; }

    | ID LIKE STR       { std::cout << "  Condition: " << $1 << " matches pattern " << $3 << std::endl; }
    | ID IN LPAREN value_list RPAREN { std::cout << "  Condition: " << $1 << " in list" << std::endl; }
    | STR IN LPAREN value_list RPAREN { std::cout << "  Condition: permanent inscription in the list." << std::endl; }
    | ID BETWEEN value AND value {
            std::cout << "  Condition: " << $1 << " between values" << std::endl;
        }

    | condition AND condition { std::cout << "  Connection: AND" << std::endl; }
    | condition OR condition  { std::cout << "  Connection: OR" << std::endl; }
    | LPAREN condition RPAREN
    ;


id_list:
    ID                  { std::cout << "Column: " << $1 << std::endl; }
    | id_list COMMA ID  { std::cout << "Column: " << $3 << std::endl; }
    ;

// CREATE TABLE
create_stmt:
    CREATE TABLE ID LPAREN column_defs RPAREN {
        std::cout << "Creating table: " << $3 << std::endl;
    }
    ;

// COLUMNS
column_defs:
    column_def
    | column_def COMMA column_def
    ;

// $1, $3 -> pseudo-identification variables in Bison
column_def:
    ID data_type {
        std::cout << "  Column added: " << $1 << std::endl;
    }
    ;

// DATA TYPES
data_type:
    TYPE_INT | TYPE_STRING | TYPE_BOOL | TYPE_DOUBLE | TYPE_FLOAT | TYPE_NULL
    ;

// INSERT INTO
insert_stmt:
    INSERT INTO ID VALUES LPAREN value_list RPAREN {
    		// this will be executed when the parser finds that the fragment matches the pattern
        std::cout << "Inserting data into the table: " << $3 << std::endl;
    }
    ;

// DELETE
delete_stmt:
    DELETE FROM ID where_clause {
        std::cout << "Removing from table " << $3 << std::endl;
    }
    ;

// UPDATE
update_stmt:
    UPDATE ID SET ID EQ value where_clause {
        std::cout << "Updating table " << $2 << ", column " << $4 << std::endl;
    }
    ;

// DROP TABLE
drop_table_stmt:
    DROP TABLE ID {
        std::cout << "Deleting table " << $3 << std::endl;
    }
    ;

// DROP COLUMN WITH ALTER
alter_table_stmt:
    ALTER TABLE ID DROP COLUMN ID {
        std::cout << "Deleting column " << $6 << " from table " << $3 << std::endl;
    }
    ;

opt_order_by:
    /* blank */
    | ORDER BY ID opt_asc_desc { std::cout << "  Sorting by: " << $3 << std::endl; }
    ;

opt_asc_desc:
    /* blank */
    | ASC
    | DESC
    ;

// Column aggregations
columns:
    STAR                { std::cout << "All columns selected (*)" << std::endl; }
    | id_list           { std::cout << "Finished loading column list." << std::endl; }
    | aggregate_func
    ;

aggregate_func:
    COUNT LPAREN STAR RPAREN { std::cout << "  Aggregation: COUNT(*)" << std::endl; }
    | SUM LPAREN ID RPAREN   { std::cout << "  Aggregation: SUM(" << $3 << ")" << std::endl; }
    | MIN LPAREN ID RPAREN   { std::cout << "  Aggregation: MIN(" << $3 << ")" << std::endl; }
    | MAX LPAREN ID RPAREN   { std::cout << "  Aggregation: MAX(" << $3 << ")" << std::endl; }
    ;

value_list:
    value
    | value_list COMMA value
    ;

value:
    NUMBER | STR | ID | TYPE_NULL
    ;

%%


// function that will be called automatically
// if the user enters something that doesn't match the grammar
void yyerror(const char *s) {
    std::cerr << "Syntax error: " << s << std::endl;
}