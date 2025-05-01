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
"int"                   { col += yyleng;  return INT; }
"return"                { col += yyleng;  return RETURN; }
"switch"                { col += yyleng;  return SWITCH; }
"then"                  { col += yyleng;  return THEN; }
"void"                  { col += yyleng;  return VOID; }
"while"                 { col += yyleng;  return WHILE; }
"break"                 { col += yyleng;  return BREAK; }

"+"                     { col += yyleng;  return PLUS; }
"-"                     { col += yyleng;  return MOINS; }
"*"                     { col += yyleng;  return MUL; }
"/"                     { col += yyleng;  return DIV; }
"&"                     { col += yyleng;  return BAND; }
"|"                     { col += yyleng;  return BOR; }
"&&"                    { col += yyleng;  return LAND; }
"||"                    { col += yyleng;  return LOR; }
"<"                     { col += yyleng;  return LT; }
">"                     { col += yyleng;  return GT; }
">="                    { col += yyleng;  return GEQ; }
"<="                    { col += yyleng;  return LEQ; }
"=="                    { col += yyleng;  return EQ; }
"!="                    { col += yyleng;  return NEQ; }
"!"                     { col += yyleng;  return NOT; }
"<<"                     { col += yyleng;  return LSHIFT; }
">>"                     { col += yyleng;  return RSHIFT; }
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

{D}+                    { yylval.integer = atoi(yytext); col += yyleng;  return CONSTANTE; }

{L}({L}|{D})*           { yylval.string = strdup(yytext); col += yyleng;  return IDENTIFICATEUR; }


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

