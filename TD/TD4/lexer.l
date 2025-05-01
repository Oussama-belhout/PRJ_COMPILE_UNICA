%option noyywrap
%option yylineno
%{
 #include <stdio.h>
 #include <stdlib.h>
 #include "parser.tab.h"
 extern int yylval; /** La variable qui contient la valeur (attribut) du token courant*/
 extern int yylineno; /* numéro de ligne du token courant */
 int yylex(void); /* prototype de la fonction lex */
 int yyerror(char* s); /* prototype d'une fonction yacc */
%}
C   [0-9]
%%
{C}+  {sscanf(yytext, "%d", &yylval); return NB; }
[ \n\t]  
"*"  {  printf("Scanned *\n");return '*';}
"+"  { printf("Scanned +\n");return '+'; }
"("  { printf("Scanned (\n");return '('; }
")"  { printf("Scanned )\n");return ')'; }
.  
%%