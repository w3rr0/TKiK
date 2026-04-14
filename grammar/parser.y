%code requires { // this code will be added to the parser.hpp
    #include <string>
    #include <vector>
    #include <memory>

    // forward declarations
    #include "ast/Statement.hpp"
    #include "ast/WhereClause.hpp"
    #include "ast/ColumnDef.hpp"
    #include "ast/SelectStmt.hpp"
    #include "ast/InsertStmt.hpp"
    #include "ast/CreateStmt.hpp"
    #include "ast/DeleteStmt.hpp"
    #include "ast/UpdateStmt.hpp"
    #include "ast/DropTableStmt.hpp"
    #include "ast/AlterTableStmt.hpp"
}

%{
#include <iostream>
#include <cstdio>

#include "ast/Statement.hpp"

int yylex();
void yyerror(const char *s);

// pointer for parsing results
Statement* root_statement = nullptr;
%}

// what tokens can store (number or text)
%union {
int num;
    char* str;
    bool boolean;
    Statement* stmt;
    WhereClause* where;
    ColumnDef* col_def;
    std::vector<ColumnDef>* col_defs;
    std::vector<std::string>* strings;
    std::vector<std::vector<std::string>>* multi_strings;
    struct {
            SelectStmt::Aggregate type;
            char* col;
    } agg;
}

// all tokens that the scanner will send and optional types in <>
%token SELECT FROM WHERE CREATE TABLE INSERT INTO VALUES UPDATE DELETE SET DROP COLUMN ALTER ADD

%token TYPE_INT TYPE_STRING TYPE_BOOL TYPE_DOUBLE TYPE_FLOAT TYPE_NULL

%token IN LIKE BETWEEN AND OR IS
%token DISTINCT COUNT SUM MIN MAX
%token ORDER BY ASC DESC LIMIT OFFSET

%token LPAREN RPAREN COMMA SEMICOLON
%token STAR EQ GT GE LE LT

%token <num> NUMBER
%token <str> ID STR

// define priorities for logical operators (prevents conflicts)
// definiotion of types for our grammar rules
%type <stmt> instrukcja select_stmt create_stmt insert_stmt update_stmt delete_stmt drop_table_stmt alter_table_stmt
%type <where> where_clause condition
%type <strings> id_list value_list columns
%type <col_defs> column_defs
%type <col_def> column_def
%type <str> data_type value opt_order_by
%type <boolean> opt_distinct opt_asc_desc
%type <agg> aggregate_func
%type <multi_strings> values_lists
%type <str> opt_column
%type <num> optional_limit optional_offset

%left OR
%left AND

%%
// 'target:' + 'ingredient1' + 'ingredient2' + ';'
program:
    instrukcja SEMICOLON {
        root_statement = $1;
        YYACCEPT;
    }
    ;

// POSSIBLE INSTRUCTIONS
instrukcja:
    select_stmt         { $$ = $1; }
    | create_stmt       { $$ = $1; }
    | insert_stmt       { $$ = $1; }
    | update_stmt       { $$ = $1; }
    | delete_stmt       { $$ = $1; }
    | drop_table_stmt   { $$ = $1; }
    | alter_table_stmt  { $$ = $1; }
    ;

// SELECT with DISTINCT and ORDER BY
select_stmt:
    SELECT opt_distinct columns FROM ID where_clause opt_order_by opt_asc_desc optional_limit optional_offset{
        // idxs: $1:SELECT, $2:opt_distinct, $3:columns, $4:FROM, $5:ID, $6:where_clause, $7:opt_order_by, $8:opt_asc_desc
        auto* sel = new SelectStmt($5, *$3, $2);

        if ($6) sel->setWhere(std::unique_ptr<WhereClause>($6));
        if ($7) {
            sel->setOrder($7, $8);
            free($7);
        }
        sel->limit = $9;
        sel->offset = $10;

        $$ = sel;
        free($5); delete $3;
    }
    | SELECT aggregate_func FROM ID where_clause opt_order_by opt_asc_desc optional_limit optional_offset {
        // $1:SELECT, $2:aggregate_func, $3:FROM, $4:ID, $5:where_clause,
        // $6:opt_order_by, $7:opt_asc_desc, $8:optional_limit, $9:optional_offset

        auto* stmt = new SelectStmt($4, {});

        // setting aggregation
        stmt->setAggregate($2.type, $2.col);

        // WHERE
        if ($5) stmt->setWhere(std::unique_ptr<WhereClause>($5));

        // ORDER BY
        if ($6) {
            stmt->setOrder($6, $7);
            free($6);
        }

        stmt->limit = $8;
        stmt->offset = $9;

        $$ = stmt;
        free($4);
        if ($2.col) free($2.col);
    }
    ;

opt_distinct:
    /* blank */ { $$ = false; }
    | DISTINCT  { $$ = true; }
    ;

where_clause:
    /* blank */ { $$ = nullptr; }
    | WHERE condition { $$ = $2; }
    ;

// examples of conditions in WHERE
condition:
    ID EQ value {
        $$ = new ComparisonCondition($1, "=", $3);
        free($1); free($3);
    }
    | ID GT value {
        $$ = new ComparisonCondition($1, ">", $3);
        free($1); free($3);
    }
    | ID LT value {
        $$ = new ComparisonCondition($1, "<", $3);
        free($1); free($3);
    }
    | ID GE value {
        $$ = new ComparisonCondition($1, ">=", $3);
        free($1); free($3);
    }
    | ID LE value {
        $$ = new ComparisonCondition($1, "<=", $3);
        free($1); free($3);
    }
    | ID IS TYPE_NULL {
        $$ = new ComparisonCondition($1, "IS", "NULL");
        free($1);
    }
    | ID LIKE STR {
        $$ = new ComparisonCondition($1, "LIKE", $3);
        free($1); free($3);
    }
    | ID IN LPAREN value_list RPAREN {
        $$ = new InCondition($1, *$4);
        free($1); delete $4;
    }
    | ID BETWEEN value AND value {
        $$ = new BetweenCondition($1, $3, $5);
        free($1); free($3); free($5);
    }
    | condition AND condition {
        $$ = new LogicalCondition(std::unique_ptr<WhereClause>($1), "AND", std::unique_ptr<WhereClause>($3));
    }
    | condition OR condition  {
        $$ = new LogicalCondition(std::unique_ptr<WhereClause>($1), "OR", std::unique_ptr<WhereClause>($3));
    }
    | LPAREN condition RPAREN {
        $$ = $2;
    }
    ;


id_list:
    ID { $$ = new std::vector<std::string>(); $$->push_back($1); free($1); }
    | id_list COMMA ID { $1->push_back($3); $$ = $1; free($3); }
    ;

// CREATE TABLE
create_stmt:
    CREATE TABLE ID LPAREN column_defs RPAREN {
        $$ = new CreateStmt($3, *$5);
        free($3); delete $5;
    }
    ;

// COLUMNS
column_defs:
    column_def { $$ = new std::vector<ColumnDef>(); $$->push_back(*$1); delete $1; }
    | column_defs COMMA column_def { $1->push_back(*$3); $$ = $1; delete $3; }
    ;

// $1, $3 -> pseudo-identification variables in Bison
column_def:
    ID data_type { $$ = new ColumnDef{$1, $2}; free($1); }
    ;

// optional column for ADD (COLUMN)
opt_column:
    /* blank */ { $$ = nullptr; }
    | COLUMN    { $$ = nullptr; }
    ;

// DATA TYPES
data_type:
    TYPE_INT      { $$ = strdup("INT"); }
    | TYPE_STRING { $$ = strdup("STRING"); }
    | TYPE_BOOL   { $$ = strdup("BOOL"); }
    | TYPE_DOUBLE { $$ = strdup("DOUBLE"); }
    | TYPE_FLOAT  { $$ = strdup("FLOAT"); }
    ;

// INSERT INTO
insert_stmt:
    INSERT INTO ID VALUES values_lists {
        $$ = new InsertStmt($3, *$5);
        delete $5;
        free($3);
    }
    ;

// many lists for inserting many rows
values_lists:
    values_lists COMMA LPAREN value_list RPAREN {
        $1->push_back(*$4); // $1 -> vector<vector<string>>*
        delete $4;          // $4 -> vector<string>*
        $$ = $1;
    }
    | LPAREN value_list RPAREN {
        $$ = new std::vector<std::vector<std::string>>();
        $$->push_back(*$2);
        delete $2;
    }
    ;

// single list for inserting one row
value_list:
    value_list COMMA value {
        $1->push_back($3);
        $$ = $1;
    }
    | value {
        $$ = new std::vector<std::string>();
        $$->push_back($1);
    }
    ;

// DELETE
delete_stmt:
    DELETE FROM ID where_clause {
        auto* del = new DeleteStmt($3);
        if ($4) del->setWhere(std::unique_ptr<WhereClause>($4));
        $$ = del;
        free($3);
    }
    ;

// UPDATE
update_stmt:
    UPDATE ID SET ID EQ value where_clause {
        auto* upd = new UpdateStmt($2, $4, $6);
        if ($7) {
            // if WHERE condition
            upd->setWhere(std::unique_ptr<WhereClause>($7));
        }
        $$ = upd;
        free($2); free($4); free($6);
    }
    ;

// DROP TABLE
drop_table_stmt:
    DROP TABLE ID { $$ = new DropTableStmt($3); free($3); }
    ;

// DROP COLUMN and ADD COLUMN
alter_table_stmt:
    ALTER TABLE ID DROP COLUMN ID {
        $$ = new AlterTableStmt($3, $6, AlterTableStmt::DROP);
        free($3); free($6);
    }
    | ALTER TABLE ID ADD opt_column ID data_type {
        Cell::Type t = Cell::TEXT;
        std::string st($7); // $7 to teraz data_type
        if (st == "INT") t = Cell::INT;
        else if (st == "DOUBLE") t = Cell::DOUBLE;
        else if (st == "BOOL") t = Cell::BOOL;

        $$ = new AlterTableStmt($3, $6, t, AlterTableStmt::ADD);

        free($3); free($6); free($7);
    }
    ;

// ORDER BY ASC / DESC
opt_order_by:
    /* blank */ { $$ = nullptr; }
    | ORDER BY ID { $$ = $3; } //column name
    ;

opt_asc_desc:
    /* blank */ { $$ = true; } // default
    | ASC       { $$ = true; }
    | DESC      { $$ = false; }
    ;

// LIMIT
optional_limit:
    LIMIT NUMBER { $$ = $2; }
    | /* empty */ { $$ = -1; }

// OFFSET
optional_offset:
    OFFSET NUMBER { $$ = $2; }
    | /* empty */ { $$ = 0; }

// Column aggregations
columns:
    STAR      { $$ = new std::vector<std::string>{"*"}; }
    | id_list { $$ = $1; }
    | aggregate_func {
        $$ = new std::vector<std::string>{"AGGREGATE_PLACEHOLDER"};
    }
    ;

aggregate_func:
      COUNT LPAREN STAR RPAREN   { $$.type = SelectStmt::Aggregate::COUNT; $$.col = strdup("*"); }
    | COUNT LPAREN ID RPAREN     { $$.type = SelectStmt::Aggregate::COUNT; $$.col = $3; }
    | SUM LPAREN ID RPAREN       { $$.type = SelectStmt::Aggregate::SUM;   $$.col = $3; }
    | MIN LPAREN ID RPAREN       { $$.type = SelectStmt::Aggregate::MIN;   $$.col = $3; }
    | MAX LPAREN ID RPAREN       { $$.type = SelectStmt::Aggregate::MAX;   $$.col = $3; }
    ;

value:
    NUMBER { $$ = strdup(std::to_string($1).c_str()); }
    | STR { $$ = $1; }
    | ID { $$ = $1; }
    | TYPE_NULL { $$ = strdup("NULL"); }
    ;

%%


// function that will be called automatically
// if the user enters something that doesn't match the grammar
void yyerror(const char *s) {
    std::cerr << "[ERROR] " << s << std::endl;
}