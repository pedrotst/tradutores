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
/*

:line 
: exp ';'
    { exp *e = $1; print_exp(e, 0); destruct_tree(a); }

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
*/

%}


%start program
%union {char c; char *strs; int num; struct exp_s* exp_v; struct term_s* term_v;}

%token ID NUM COMMA SEMICOLON VAR_ATTRIBUITION L_PAREN R_PAREN
%token L_BRACK R_BRACK DOT
%token NOT BAND BOR BEQ BGE BLE BGT BLT PRINT
%token INT BOOL THIS NEW CLASS OBJECT TRUE FALSE RETURN SUPER EXTENDS IF ELSE WHILE

%left '+' 
%left '-'
%left '*' 
%left '/'
%left BAND
%left BOR
%right NOT
%left BEQ BGE BLE BGT BLT

%type <num> NUM TRUE FALSE

%type <strs> ID

%%
program 
: ClassDecl StmtList    {
    }    

ClassDecl
: %empty
| CLASS ID EXTENDS ClassName L_BRACK VarDecl Constructor MethodDecl R_BRACK ClassDecl{
    }

Constructor
: ID L_PAREN FormalArgs R_PAREN L_BRACK SUPER L_PAREN ArgList R_PAREN SEMICOLON Suite R_BRACK {
    }

MethodDecl
: %empty
| Type ID L_PAREN FormalArgs R_PAREN L_BRACK Suite RETURN L_PAREN Exp R_PAREN R_BRACK MethodDecl{
}

ClassName
: OBJECT {}
| ID {}

Assignment
: ID VAR_ATTRIBUITION Exp SEMICOLON{}

Var
: THIS {}
| ID {}

Suite
: L_BRACK StmtList R_BRACK {}
| Stmt SEMICOLON {}

StmtList
: %empty
| Stmt SEMICOLON StmtList {}

ArgList
: %empty {}
| Exp COMMA ArgList {}


FormalArgs
: %empty {}
| Type ID COMMA FormalArgs{}

VarDecl
: Type IdList SEMICOLON {}

IdList
: ID 
| ID COMMA IdList {}


Type
: ClassName {}
| INT {}
| BOOL {}

Exp
: Var {}
| FieldAccess {}
| MethodInvoc {}
| New {}
| Assignment {}
| Int {}
| Bool {}

FieldAccess
: Exp DOT ID {}

MethodInvoc
: Exp DOT ID L_PAREN ArgList R_PAREN {}

New
: NEW ID L_PAREN ArgList R_PAREN {}

Int
: Int '+' Int {}
| Int '-' Int {}
| Int '*' Int {}
| Int '/' Int {}
| NUM
| Exp

Bool
: BOOL BOR BOOL
| BOOL BAND BOOL
| NOT BOOL
| Int BEQ Int {}
| Int BLE Int {}
| Int BGE Int {}
| Int BLT Int {}
| Int BGT Int {}
| TRUE 
| FALSE
| Exp

Stmt
: IF BOOL Suite {}
| MatchedStmt {}

MatchedStmt
: IF Bool Suite ELSE Suite {}
| WHILE Bool Suite {}
| VarDecl {}
| Exp SEMICOLON {} 


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


