FLAGS = -g 

all: lex yacc sTable ast linkAll moveObjs 	

linkAll:
	gcc $(FLAGS) -o fj lex.yy.c y.tab.c ast.o symbol_table.o

yacc:
	yacc -d --report=lookahead -v ./src/fj.y

lex:
	lex ./src/fj.l 

ast:
	gcc $(FLAGS) -c ./bib/ast.c

sTable:
	gcc $(FLAGS) -c ./bib/symbol_table.c

moveObjs:
	mkdir -p obj
	mv lex.yy.c ./obj/lex.yy.c
	mv y.tab.c ./obj/y.tab.c
	mv symbol_table.o ./obj/symbol_table.o
	mv ast.o ./obj/ast.o
	mv y.tab.h ./obj/y.tab.h
