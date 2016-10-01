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
%union {
    char c; 
    char *strs;
    int num; 
    struct Program_s*       program_v;
    struct ClassDecl_s*     class_v;
    struct Constructor_s*   constructor_v; 
    struct MethodDecl_s*    methodDecl_v; 
    struct ClassName_s*     className_v; 
    struct Assignment_s*    assignment_v; 
    struct Var_s*           var_v; 
    struct Suite_s*         suite_v; 
    struct StmtList_s*      stmtList_v;
    struct ArgList_s*       argList_v; 
    struct FormalArgs_s*    formalArgs_v; 
    struct VarDecl_s*       varDecl_v; 
    struct IdList_s*        idList_v; 
    struct Type_s*          type_v; 
    struct Exp_s*           exp_v; 
    struct FieldAccess_s*   fieldAccess_v; 
    struct MethodInvoc_s*   methodInvoc_v; 
    struct New_s*           new_v; 
    struct Int_s*           int_v; 
    struct Bool_s*          bool_v; 
    struct Stmt_s*          stmt_v; 
    struct MatchedStmt_s*   matchedStmt_v; 

}

%token ID NUM COMMA SEMICOLON VAR_ATTRIBUITION L_PAREN R_PAREN
%token L_BRACK R_BRACK DOT
%token NOT BAND BOR BEQ BGE BLE BGT BLT PRINT
%token INT BOOL THIS NEW CLASS OBJECT TRUE FALSE RETURN SUPER EXTENDS IF ELSE WHILE

%left '+' '-'
%left '*' '/'
%left BAND
%left BOR
%right NOT
%left BEQ BGE BLE BGT BLT

%type <program_v> program
%type <class_v> classDecl
%type <constructor_v> constructor
%type <methodDecl_v> methodDecl
%type <className_v> className
%type <assignment_v> assignment
%type <var_v> var
%type <suite_v> suite
%type <stmtList_v>stmtList
%type <argList_v> argList
%type <formalArgs_v> formalArgs
%type <varDecl_v> varDecl
%type <idList_v> idList ID
%type <type_v> type
%type <exp_v> exp
%type <fieldAccess_v> fieldAccess
%type <methodInvoc_v> methodInvoc
%type <new_v> new
%type <int_v> int NUM
%type <bool_v> bool TRUE FALSE NOT
%type <stmt_v> stmt
%type <matchedStmt_v> matchedStmt

%%
program 
: classDecl stmtList    {
    $$ = program_node($1, $2);
}    

classDecl
: %empty {$$ = NULL;}
| CLASS ID EXTENDS className L_BRACK varDecl constructor methodDecl R_BRACK classDecl{
    $$ = NULL;
}

constructor
: ID L_PAREN formalArgs R_PAREN L_BRACK SUPER L_PAREN argList R_PAREN SEMICOLON suite R_BRACK {
    }

methodDecl
: %empty {$$ = NULL;}
| type ID L_PAREN formalArgs R_PAREN L_BRACK suite RETURN L_PAREN exp R_PAREN R_BRACK methodDecl{
}

className
: OBJECT {}
| ID {}

assignment
: ID VAR_ATTRIBUITION exp SEMICOLON{}

var
: THIS {}
| ID {}

suite
: L_BRACK stmtList R_BRACK {}
| stmt SEMICOLON {}

stmtList
: %empty {$$ = NULL;}
| stmt SEMICOLON stmtList {$$ = NULL;}

argList
: %empty {}
| exp COMMA argList {}


formalArgs
: %empty {$$ = NULL;}
| type ID COMMA formalArgs{}

varDecl
: type idList SEMICOLON {}

idList
: ID 
| ID COMMA idList {}


type
: className {}
| INT {}
| BOOL {}

exp
: var {}
| fieldAccess {}
| methodInvoc {}
| new {}
| assignment {}
| int {}
| bool {}

fieldAccess
: exp DOT ID {}

methodInvoc
: exp DOT ID L_PAREN argList R_PAREN {}

new
: NEW ID L_PAREN argList R_PAREN {}

int
: int '+' int {}
| int '-' int {}
| int '*' int {}
| int '/' int {}
| NUM

bool
: bool BOR bool
| bool BAND bool
| NOT bool
| int BEQ int {}
| int BLE int {}
| int BGE int {}
| int BLT int {}
| int BGT int {}
| TRUE 
| FALSE

stmt
: IF BOOL suite {}
| matchedStmt {}

matchedStmt
: IF bool suite ELSE suite {}
| WHILE bool suite {}
| varDecl {}
| exp SEMICOLON {} 


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


