D                       [0-9]
L                       [a-zA-Z_]
H                       [a-fA-F0-9]
E                       [Ee][+-]?{D}+
FS                      (f|F|l|L)
IS                      (u|U|l|L)*

%{
#include "miniC.tab.h"
#include <stdio.h>
#include <string.h>

extern YYSTYPE yylval;
extern int nb_line;
extern int col;



/* Ne gere pas les commentaires. A rajouter */
/* Supprimer les lignes dont vous n'avez pas besoin. */

%}

%%
"case"                  { col += yyleng; printf("LEXER: Matched token CASE: '%s'\n", yytext); return CASE; }
"const"                 { col += yyleng; printf("LEXER: Matched token CONSTANTE: '%s'\n", yytext); return CONSTANTE; }
"default"               { col += yyleng; printf("LEXER: Matched token DEFAULT: '%s'\n", yytext); return DEFAULT; }
"else"                  { col += yyleng; printf("LEXER: Matched token ELSE: '%s'\n", yytext); return ELSE; }
"extern"                { col += yyleng; printf("LEXER: Matched token EXTERN: '%s'\n", yytext); return EXTERN; }
"for"                   { col += yyleng; printf("LEXER: Matched token FOR: '%s'\n", yytext); return FOR; }
"if"                    { col += yyleng; printf("LEXER: Matched token IF: '%s'\n", yytext); return IF; }
"int"                   { col += yyleng; printf("LEXER: Matched token INT: '%s'\n", yytext); return INT; }
"return"                { col += yyleng; printf("LEXER: Matched token RETURN: '%s'\n", yytext); return RETURN; }
"switch"                { col += yyleng; printf("LEXER: Matched token SWITCH: '%s'\n", yytext); return SWITCH; }
"then"                  { col += yyleng; printf("LEXER: Matched token THEN: '%s'\n", yytext); return THEN; }
"void"                  { col += yyleng; printf("LEXER: Matched token VOID: '%s'\n", yytext); return VOID; }
"while"                 { col += yyleng; printf("LEXER: Matched token WHILE: '%s'\n", yytext); return WHILE; }
"break"                 { col += yyleng; printf("LEXER: Matched token BREAK: '%s'\n", yytext); return BREAK; }

"+"                     { col += yyleng; printf("LEXER: Matched token PLUS: '%s'\n", yytext); return PLUS; }
"-"                     { col += yyleng; printf("LEXER: Matched token MOINS: '%s'\n", yytext); return MOINS; }
"*"                     { col += yyleng; printf("LEXER: Matched token MUL: '%s'\n", yytext); return MUL; }
"/"                     { col += yyleng; printf("LEXER: Matched token DIV: '%s'\n", yytext); return DIV; }
"&"                     { col += yyleng; printf("LEXER: Matched token BAND: '%s'\n", yytext); return BAND; }
"|"                     { col += yyleng; printf("LEXER: Matched token BOR: '%s'\n", yytext); return BOR; }
"&&"                    { col += yyleng; printf("LEXER: Matched token LAND: '%s'\n", yytext); return LAND; }
"||"                    { col += yyleng; printf("LEXER: Matched token LOR: '%s'\n", yytext); return LOR; }
"<"                     { col += yyleng; printf("LEXER: Matched token LT: '%s'\n", yytext); return LT; }
">"                     { col += yyleng; printf("LEXER: Matched token GT: '%s'\n", yytext); return GT; }
">="                    { col += yyleng; printf("LEXER: Matched token GEQ: '%s'\n", yytext); return GEQ; }
"<="                    { col += yyleng; printf("LEXER: Matched token LEQ: '%s'\n", yytext); return LEQ; }
"=="                    { col += yyleng; printf("LEXER: Matched token EQ: '%s'\n", yytext); return EQ; }
"!="                    { col += yyleng; printf("LEXER: Matched token NEQ: '%s'\n", yytext); return NEQ; }
"!"                     { col += yyleng; printf("LEXER: Matched token NOT: '%s'\n", yytext); return NOT; }
"="                     { col += yyleng; printf("LEXER: Matched token OP: '%s'\n", yytext); return OP; }
";"                     { col += yyleng; printf("LEXER: Matched token SEMICOLON: '%s'\n", yytext); return SEMICOLON; }
"{"                     { col += yyleng; printf("LEXER: Matched token LBRACE: '%s'\n", yytext); return LBRACE; }
"}"                     { col += yyleng; printf("LEXER: Matched token RBRACE: '%s'\n", yytext); return RBRACE; }
"("                     { col += yyleng; printf("LEXER: Matched token LPAREN: '%s'\n", yytext); return LPAREN; }
")"                     { col += yyleng; printf("LEXER: Matched token RPAREN: '%s'\n", yytext); return RPAREN; }
","                     { col += yyleng; printf("LEXER: Matched token COMMA: '%s'\n", yytext); return COMMA; }
"["                     { col += yyleng; printf("LEXER: Matched token LBRACKET: '%s'\n", yytext); return LBRACKET; }
"]"                     { col += yyleng; printf("LEXER: Matched token RBRACKET: '%s'\n", yytext); return RBRACKET; }

{D}+                    { yylval.integer = atoi(yytext); col += yyleng; printf("LEXER: Matched token CONSTANTE: '%s' (value: %d)\n", yytext, yylval.integer); return CONSTANTE; }

{L}({L}|{D})*           { yylval.string = strdup(yytext); col += yyleng; printf("LEXER: Matched token IDENTIFICATEUR: '%s'\n", yytext); return IDENTIFICATEUR; }


L?\"(\\.|[^\\"])*\"     { /* A completer */ }


[ \t\v\n\f]             { /* A completer */ }

"/*"(.|\n)*?"*/"        { 
                            int i;
                            for (i = 0; i < yyleng; i++) {
                                if (yytext[i] == '\n') {
                                    nb_line++;
                                    col = 1;
                                } else {
                                    col++;
                                }
                            }
                        }

. {printf("\nErreur lexicale : %s a ligne %d colonne %d \n", yytext, nb_line, col); col = col + yyleng;}

%%

