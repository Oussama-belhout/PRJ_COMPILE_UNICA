%{
#include "miniC.tab.h"
#include <stdio.h>
#include <string.h>
#include "symtab.h"

extern YYSTYPE yylval;
extern int nb_line;
extern int col;



/* Ne gere pas les commentaires. A rajouter */
/* Supprimer les lignes dont vous n'avez pas besoin. */

%}

D                       [0-9]
L                       [a-zA-Z_]
H                       [a-fA-F0-9]
E                       [Ee][+-]?{D}+
FS                      (f|F|l|L)
IS                      (u|U|l|L)*

%%
"case"                  { col += yyleng;  return CASE; }
"const"                 { col += yyleng;  return CONSTANTE; }
"default"               { col += yyleng;  return DEFAULT; }
"else"                  { col += yyleng;  return ELSE; }
"extern"                { col += yyleng;  return EXTERN; }
"for"                   { col += yyleng;  return FOR; }
"if"                    { col += yyleng;  return IF; }
"int"                   { yylval.str = strdup(yytext); col += yyleng;  return INT; }
"return"                { col += yyleng;  return RETURN; }
"switch"                { col += yyleng;  return SWITCH; }
"then"                  { col += yyleng;  return THEN; }
"void"                  { yylval.str = strdup(yytext); col += yyleng;  return VOID; }
"while"                 { col += yyleng;  return WHILE; }
"break"                 { col += yyleng;  return BREAK; }

"+"                     { yylval.str = strdup(yytext); col += yyleng; return PLUS; }
"-"                     { yylval.str = strdup(yytext); col += yyleng; return MOINS; }
"*"                     { yylval.str = strdup(yytext); col += yyleng; return MUL; }
"/"                     { yylval.str = strdup(yytext); col += yyleng; return DIV; }
"&"                     { yylval.str = strdup(yytext); col += yyleng; return BAND; }
"|"                     { yylval.str = strdup(yytext); col += yyleng; return BOR; }
"&&"                    { yylval.str = strdup(yytext); col += yyleng; return LAND; }
"||"                    { yylval.str = strdup(yytext); col += yyleng; return LOR; }
"<"                     { yylval.str = strdup(yytext); col += yyleng; return LT; }
">"                     { yylval.str = strdup(yytext); col += yyleng; return GT; }
">="                    { yylval.str = strdup(yytext); col += yyleng; return GEQ; }
"<="                    { yylval.str = strdup(yytext); col += yyleng; return LEQ; }
"=="                    { yylval.str = strdup(yytext); col += yyleng; return EQ; }
"!="                    { yylval.str = strdup(yytext); col += yyleng; return NEQ; }
"!"                     { yylval.str = strdup(yytext); col += yyleng; return NOT; }
"<<"                     { yylval.str = strdup(yytext); col += yyleng; return LSHIFT; }
">>"                     { yylval.str = strdup(yytext); col += yyleng; return RSHIFT; }
"="                     { col += yyleng;  return OP; }
";"                     { col += yyleng;  return SEMICOLON; }
":"                     { col += yyleng;  return COLON; }
"{"                     { col += yyleng;  return LBRACE; }
"}"                     { col += yyleng;  return RBRACE; }
"("                     { col += yyleng;  return LPAREN; }
")"                     { col += yyleng;  return RPAREN; }
","                     { col += yyleng;  return COMMA; }
"["                     { col += yyleng;  return LBRACKET; }
"]"                     { col += yyleng;  return RBRACKET; }

{D}+                    { yylval.integer = atoi(yytext);col += yyleng;  return CONSTANTE; }

{L}({L}|{D})*           { yylval.str = strdup(yytext); col += yyleng;  return IDENTIFICATEUR; }


L?\"(\\.|[^\\"])*\"     { /* A completer */ }


[ \t\v\f]             { /* A completer */ }

\n { col = 1 ; nb_line++;} 

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

