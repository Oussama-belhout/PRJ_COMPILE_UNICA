%debug
%{

 #include <stdio.h>
 #include <stdlib.h>
 extern int yylineno; /* numéro de ligne du token courant */
 extern int yydebug;
 int yylex(void);
 int yyerror(char *s);
 %}

 %token   NB

 %
 %%
 S:
 E {printf("résultat=%d\n", $1);}
 E:
 E '+' T {$$= $1 + $3;}
 | T    {$$= $1;}
 T:
   T '*' F  {$$= $1 * $3;}
 | F        {$$= $1;}
 F:
  '(' E ')' {$$= $2;}
 | NB  {$$= $1;}
 %%
 int yyerror( char *s ) {
  fprintf( stderr, "%s. Ligne:%d\n", s, yylineno );
  exit(1);
 }
 int main() {
    yydebug = 1 ;
    yyparse();
    return (0);
 }