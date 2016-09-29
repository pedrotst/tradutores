all: yacc lex
	gcc -o fj lex.yy.c y.tab.c

yacc:
	yacc -d fj.y

lex:
	lex fj.l 
