flex lexer.l
bison -d parser.y
gcc lex.yy.c parser.tab.c -lfl -o compiler
compiler.exe < test.txt
