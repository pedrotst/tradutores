FLAGS = -g 

all: yacc lex ast
	gcc $(FLAGS) -o fj lex.yy.c y.tab.c ast.o

yacc:
	yacc -d fj.y

lex:
	lex fj.l 

ast:
	gcc $(FLAGS) -c ./bib/ast.c