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
%union {int num; struct ast_s* ast_v;}

%token ID NUM COMMA SEMICOLON VAR_ATTRIBUITION L_PAREN R_PAREN
%token L_CURL_BRACKETS R_CURL_BRACKETS DOT PLUS MINUS MULT DIV
%token NOT BAND BOR BEQ BGE BLE BGT BL PRINT

%type <num> NUM 
%type <ast_v> line exp term

%%
line    
: exp ';'
    { ast *a = $1; print_ast(a, 0); destruct_tree(a); }

exp     
: term                  
    {symbol s; s.tag = -1;
    $$ = node (s, EXP_T, $1, NULL);}
| exp '+' term          
    {symbol s; s.tag = 1; s.u.op='+';
    $$ = node (s, EXP_T, $1 , $3);}
| exp '-' term          
    {symbol s; s.tag = 1; s.u.op='-';
    $$ = node (s, EXP_T, $1 , $3);}
;

term    
: NUM                   
    {symbol s; s.tag = 0; s.u.num=$1;
    $$ = leafnode(s);}
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


