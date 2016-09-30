%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include "bib/ast.h"

int count_lines = 1, chars = 0;

void yyerror(const char*);
int yywrap();
int yylex(void);
%}


%start line
%union {int num; struct exp_s* exp_v; struct term_s* term_v;}

%token ID NUM COMMA SEMICOLON VAR_ATTRIBUITION L_PAREN R_PAREN
%token L_CURL_BRACKETS R_CURL_BRACKETS DOT PLUS MINUS MULT DIV
%token NOT BAND BOR BEQ BGE BLE BGT BL PRINT

%type <num> NUM 
%type <term_v> term
%type <exp_v> line exp

%%
line    
: exp ';'
    { exp *e = $1; print_exp(e, 0); /*destruct_tree(a); */}

exp     
: term                  
    {$$ = exp_termnode ($1);}
| exp '+' term          
    {$$ = exp_opnode ('-', $1 , $3);}
| exp '-' term          
    {$$ = exp_opnode ('-', $1 , $3);}
;

term    
: NUM                   
    {$$ = term_node($1);}
;

%%


void yyerror(const char *str)
{
    fprintf(stderr,"error: %s\n",str);
}

int yywrap() {return 1;};

int main()
{
    yyparse();
    return 0;
} 


