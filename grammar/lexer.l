%{
    // dołączamy biblioteki i plik .hpp
#include "parser.hpp"
#include <iostream>
#include <string.h>

    // funkcja, którą Bison woła, aby dostać kolejny token
int yylex();
%}

    /* po przeczytaniu jednego pliku ma zakończyć pracę */
%option noyywrap

%% /* oddziela definicje od reguł */
"SELECT"    { return SELECT; }
"FROM"      { return FROM; }
"WHERE"     { return WHERE; }

"CREATE"    { return CREATE; }
"TABLE"     { return TABLE; }
"INSERT"    { return INSERT; }
"INTO"      { return INTO; }
"VALUES"    { return VALUES; }
"UPDATE"    { return UPDATE; }
"DELETE"    { return DELETE; }
"SET"       { return SET; }

"IN"        { return IN; }
"LIKE"      { return LIKE; }
"BETWEEN"   { return BETWEEN; }

"ORDER"     { return ORDER; }
"BY"        { return BY; }
"ASC"       { return ASC; }
"DESC"      { return DESC; }

"DISTINCT"  { return DISTINCT; }
"COUNT"     { return COUNT; }
"SUM"       { return SUM; }
"MIN"        { return MIN; }
"MAX"        { return MAX; }

    /* Typy danych dla CREATE TABLE */
"INT"       { return TYPE_INT; }
"STRING"    { return TYPE_STRING; }
"TEXT"      { return TYPE_STRING; }
"VARCHAR"    { return TYPE_STRING; }
"BOOLEAN"    { return TYPE_BOOL; }
"DOUBLE"     { return TYPE_DOUBLE; }
"FLOAT"      { return TYPE_FLOAT; }

"*"         { return STAR; }
"="         { return EQ; }
">"         { return GT; }
">="        { return GE; }
"<="        { return LE; }
"<"         { return LT; }
"AND"       { return AND; }
"OR"        { return OR; }

"("         { return LPAREN; }
")"         { return RPAREN; }
","         { return COMMA; }
";"         { return SEMICOLON; }

    /* yytext, yylval -> wbudowane zmienne Flexa */
[0-9]+      { yylval.num = atoi(yytext); return NUMBER; }

[a-zA-Z_][a-zA-Z0-9_]* {
    yylval.str = strdup(yytext); // kopiuje tekst do yylval.str
    return ID;
}

\'[^\']*\'  {
    yylval.str = strdup(yytext);
    return STR;
}

[ \t\n]+    { /* ignoruj */ }
.           { std::cerr << "Nieznany znak: " << yytext << std::endl; }
%%