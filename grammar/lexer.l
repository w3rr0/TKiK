%{
#include <iostream>
#include <string.h>

#include "ast/Statement.hpp"
#include "ast/SelectStmt.hpp"
#include "ast/WhereClause.hpp"
#include "ast/ColumnDef.hpp"

#include "parser.hpp"

/* function that Bison calls to get the next token */
int yylex();
%}

/* after reading one file it should exit */
%option noyywrap
%option caseless

%% /* separates definitions from rules*/
    /* classic SELECT */
"SELECT"    { return SELECT; }
"DISTINCT"  { return DISTINCT; }
"FROM"      { return FROM; }
"WHERE"     { return WHERE; }

    /* CREATE TABLE / UPDATE / DELETE / INSERT INTO / DROP */
"CREATE"    { return CREATE; }
"TABLE"     { return TABLE; }
"INSERT"    { return INSERT; }
"INTO"      { return INTO; }
"VALUES"    { return VALUES; }
"UPDATE"    { return UPDATE; }
"DELETE"    { return DELETE; }
"SET"       { return SET; }
"DROP"      { return DROP; }
"COLUMN"    { return COLUMN; }
"ALTER"     { return ALTER; }

    /* OPERATIONS WITH STRINGS */
"IN"        { return IN; }
"LIKE"      { return LIKE; }
"BETWEEN"   { return BETWEEN; }
"IS" { return IS; }

    /* ORDER BY */
"ORDER"     { return ORDER; }
"BY"        { return BY; }
"ASC"       { return ASC; }
"DESC"      { return DESC; }

    /* AGGREGATE FUNCTIONS */
"COUNT"     { return COUNT; }
"SUM"       { return SUM; }
"MIN"        { return MIN; }
"MAX"        { return MAX; }

    /* data types for CREATE TABLE*/
"INT"       { return TYPE_INT; }
"STRING"    { return TYPE_STRING; }
"TEXT"      { return TYPE_STRING; }
"VARCHAR"    { return TYPE_STRING; }
"BOOLEAN"    { return TYPE_BOOL; }
"DOUBLE"     { return TYPE_DOUBLE; }
"FLOAT"      { return TYPE_FLOAT; }
"NULL"      { return TYPE_NULL; }

    /* OPERATORS */
"*"         { return STAR; }
"="         { return EQ; }
">"         { return GT; }
">="        { return GE; }
"<="        { return LE; }
"<"         { return LT; }
"AND"       { return AND; }
"OR"        { return OR; }

    /* PARENTHESES / COMMA / SEMICOLON */
"("         { return LPAREN; }
")"         { return RPAREN; }
","         { return COMMA; }
";"         { return SEMICOLON; }


    /* float numbers */
[0-9]+\.[0-9]+ {
    yylval.str = strdup(yytext);
    return STR;
}

    /* yytext, yylval -> built-in Flex variables */
[0-9]+ {
    yylval.num = atoi(yytext);
    return NUMBER;
}

    /* LETTERS / NUMBERS -> REGEX */
[a-zA-Z_][a-zA-Z0-9_]* {
    std::string s(yytext);
    for(auto & c : s) c = (unsigned char)std::tolower(c);
    yylval.str = strdup(s.c_str());
    return ID;
}

    /* "" and '' service*/
(\'[^\']*\'|\"[^\"]*\") {
    yylval.str = strdup(yytext);
    return STR;
}

    /* WHITE SPACES -> skip */
[ \t\n]+    { /* ignore */ }
.           { std::cerr << "Unknown char " << yytext << std::endl; }

%%