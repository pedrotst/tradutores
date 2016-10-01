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
    struct ClassMembers_s*  classMembers_v;
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
    struct Return_s*        return_v; 
    struct FieldAccess_s*   fieldAccess_v; 
    struct MethodInvoc_s*   methodInvoc_v; 
    struct New_s*           new_v; 
    struct Int_s*           int_v; 
    struct Bool_s*          bool_v; 
    struct Stmt_s*          stmt_v; 
    struct MatchedStmt_s*   matchedStmt_v; 

}
%define parse.error verbose

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

%type <strs> ID
%type <program_v> program
%type <class_v> classDecl
%type <constructor_v> constructor
%type <methodDecl_v> methodDecl
%type <className_v> className
%type <classMembers_v> classMembers
%type <assignment_v> assignment
%type <var_v> var
%type <suite_v> suite
%type <stmtList_v>stmtList
%type <argList_v> argList
%type <formalArgs_v> formalArgs
%type <varDecl_v> varDecl
%type <idList_v> idList
%type <type_v> type
%type <exp_v> exp
%type <return_v> return
%type <fieldAccess_v> fieldAccess
%type <methodInvoc_v> methodInvoc
%type <new_v> new
%type <int_v> int NUM
%type <bool_v> bool TRUE FALSE NOT
%type <stmt_v> stmt
%type <matchedStmt_v> matchedStmt

%%
program 
: classDecl stmtList {
    Program *p; p = program_node($1, $2); print_program(p);
    $$ = p;
}    

classDecl
: %empty {$$ = NULL;}
| CLASS ID EXTENDS className L_BRACK classMembers R_BRACK classDecl{
    $$ = classDecl_node($2, $4, $6, $8);
}

classMembers
: %empty {$$ = NULL;}
| varDecl classMembers {$$ = NULL;}
| constructor classMembers {$$ = NULL;}
| methodDecl classMembers {$$ = NULL;}

constructor
: ID L_PAREN formalArgs R_PAREN L_BRACK stmtList R_BRACK {$$=NULL; }

methodDecl
: type ID L_PAREN formalArgs R_PAREN L_BRACK stmtList R_BRACK methodDecl{$$=NULL;
}

className
: OBJECT {$$=NULL;}
| ID {$$=NULL;}

assignment
: ID VAR_ATTRIBUITION exp SEMICOLON{$$=NULL;}

var
: THIS {$$=NULL;}
| ID {$$=NULL;}

suite
: L_BRACK stmtList R_BRACK {$$=NULL;}
| stmt SEMICOLON {$$=NULL;}

stmtList
: %empty {$$ = NULL;}
| stmt stmtList {$$ = NULL;}

argList
: %empty {$$=NULL;}
| exp COMMA argList {$$=NULL;}


formalArgs
: %empty {$$=NULL;}
| type ID COMMA formalArgs{$$=NULL;}

varDecl
: type idList SEMICOLON {$$=NULL;}

idList
: ID {$$=NULL;}
| idList COMMA ID {$$=NULL;}


type
: className {$$=NULL;}
| INT {$$=NULL;}
| BOOL {$$=NULL;}

exp
: var {$$=NULL;}
| fieldAccess {$$=NULL;}
| methodInvoc {$$=NULL;}
| new {$$=NULL;}
| assignment {$$=NULL;}
| return {$$=NULL;}
| int {$$=NULL;}
| bool {$$=NULL;}

return
: RETURN L_PAREN exp R_PAREN {$$=NULL;}

fieldAccess
: exp DOT ID {$$=NULL;}

methodInvoc
: exp DOT ID L_PAREN argList R_PAREN {$$=NULL;}
| ID L_PAREN argList R_PAREN {$$=NULL;}


new
: NEW ID L_PAREN argList R_PAREN {$$=NULL;}

int
: int '+' int {$$=NULL;}
| int '-' int {$$=NULL;}
| int '*' int {$$=NULL;}
| int '/' int {$$=NULL;}
| NUM{$$=NULL;}

bool
: bool BOR bool{$$=NULL;}
| bool BAND bool{$$=NULL;}
| NOT bool{$$=NULL;}
| int BEQ int {$$=NULL;}
| int BLE int {$$=NULL;}
| int BGE int {$$=NULL;}
| int BLT int {$$=NULL;}
| int BGT int {$$=NULL;}
| TRUE {$$=NULL;}
| FALSE{$$=NULL;}

stmt
: IF BOOL suite {$$=NULL;}
| matchedStmt {$$=NULL;}

matchedStmt
: IF bool suite ELSE suite {$$=NULL;}
| WHILE bool suite {$$=NULL;}
| varDecl {$$=NULL;}
| exp SEMICOLON {$$=NULL;} 


%%


void yyerror(const char *str)
{
    fprintf(stderr,"error line: %d:%d: %s\n",count_lines, chars, str);
}

int yywrap() {return 1;};

int main()
{
    yyparse();
    return 0;
} 


