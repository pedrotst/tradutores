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


%start program
%union {
    char c; 
    char *strs;
    int num; 
    struct Program_s*       program_v;
    struct ClassDecl_s*     class_v;
    struct ClassMembers_s*  classMembers_v;
    struct ClassMember_s*        classMember_v;
    struct ConstrDecl_s*        constrDecl_v;
    struct FunctionDecl_s*       functionDecl_v;
    //struct Constructor_s*   constructor_v; 
    //struct MethodDecl_s*    methodDecl_v; 
//    struct ClassName_s*     className_v; 
    struct Assignment_s*    assignment_v; 
    struct Var_s*           var_v; 
    struct Suite_s*         suite_v; 
    struct StmtList_s*      stmtList_v;
    struct ArgList_s*       argList_v; 
    struct FormalArgs_s*    formalArgs_v; 
    struct VarDecl_s*       varDecl_v; 
    struct IdList_s*        idList_v; 
    struct Exp_s*           exp_v; 
    struct Return_s*        return_v; 
    struct FieldAccess_s*   fieldAccess_v; 
    struct MethodInvoc_s*   methodInvoc_v; 
    struct New_s*           new_v; 
    struct Int_s*           int_v; 
    struct Bool_s*          bool_v; 
    struct Stmt_s*          stmt_v; 
    struct MatchedStmt_s*   matchedStmt_v; 
    struct Object_s*        object_v; 

}
%define parse.error verbose

%token ID NUM COMMA DOT
%token NOT BAND BOR BEQ BGE BLE BGT BLT PRINT
%token INT BOOL THIS NEW CLASS OBJECT TRUE FALSE RETURN SUPER EXTENDS IF ELSE WHILE

%left '+' '-'
%left '*' '/'
%left BAND
%left BOR
%right NOT
%left BEQ BGE BLE BGT BLT
%right VAR_ATTRIBUITION

%type <strs> ID INT BOOL type
%type <program_v> program 
%type <class_v> classDecl
%type <classMembers_v> classMembers
%type <classMember_v> classMember
%type <constrDecl_v> constrDecl
%type <functionDecl_v> functionDecl
%type <assignment_v> assignment
%type <var_v> var 
%type <suite_v> suite
%type <stmtList_v>stmtList
%type <argList_v> argList
%type <formalArgs_v> formalArgs
%type <varDecl_v> varDecl
%type <idList_v> idList
%type <exp_v> exp
%type <object_v> object
%type <return_v> return
%type <fieldAccess_v> fieldAccess
%type <methodInvoc_v> methodInvoc
%type <new_v> new
%type <int_v> int NUM
%type <bool_v> bool TRUE FALSE NOT
%type <stmt_v> stmt
%type <matchedStmt_v> matchedStmt
%nonassoc VARD
%nonassoc FDECL

%%
program 
: classDecl stmtList {
    Program *p; p = program_node($1, $2); 
    print_program(p);
    $$ = p;
}    
;

classDecl
: %empty {$$ = NULL;}
| classDecl CLASS ID EXTENDS ID '{' classMembers '}' {
    $$ = classDecl_node($3, $5, $7, $1);
}
| classDecl CLASS ID EXTENDS ID '{' '}' {
    $$ = classDecl_node($3, $5, NULL, $1);
}


classMembers
: classMember {$$ = classMembers_node($1, NULL);}
| classMembers classMember {$$ = classMembers_node($2, $1);}

classMember
: varDecl ';' {$$ = classMember_node(VAR_DECL, $1, NULL, NULL);}
| constrDecl {$$ = classMember_node(CONSTR_DECL, NULL, NULL, $1);}
| functionDecl {$$ = classMember_node(FUN_DECL, NULL, $1, NULL);}

constrDecl
:  ID '(' formalArgs ')' '{' stmtList '}' {$$ = constrDecl_node($1, $3, $6); }
;

functionDecl
: type ID '(' formalArgs ')' '{' stmtList '}' {$$ = functionDecl_node ($1, $2, $4, $7);}
;


assignment
: ID '=' exp {$$=NULL;}
;

var
: THIS {$$=NULL;}
| ID {$$=NULL;}
;

suite
: '{' stmtList '}' {$$=NULL;}
| stmt ';' {$$=NULL;}
;

stmtList
: %empty {$$ = NULL;}
| stmtList stmt {$$ = NULL;}
;

argList
: %empty {$$=NULL;}
| argList exp ',' {$$=NULL;}
;


formalArgs
: %empty {$$=NULL;}
| formalArgs type ID ',' {$$=NULL;}
;

varDecl
: type ID idList {$$ = varDecl_node($1, $2, $3);}
;

idList
: %empty {$$= NULL;}
| idList ',' ID {$$=idList_node($3, $1);}
;


type
: ID {$$=$1;}
| INT {$$=$1;}
| BOOL {$$=$1;}
;

exp
: object {$$=NULL;}
| int {$$=NULL;}
| bool {$$=NULL;}
| assignment {$$=NULL;}
;

object
: var {$$=NULL;}
| fieldAccess {$$=NULL;}
| methodInvoc {$$=NULL;}
| new {$$=NULL;}
;

return
: RETURN '(' exp ')' {$$=NULL;}
;

methodInvoc
: object DOT ID '(' argList ')' {$$=NULL;}
| ID '(' argList ')' {$$=NULL;}
;

fieldAccess
: object DOT ID {$$=NULL;}
;

new
: NEW ID '(' argList ')' {$$=NULL;}
;

int
: int '+' int {$$=NULL;}
| int '-' int {$$=NULL;}
| int '*' int {$$=NULL;}
| int '/' int {$$=NULL;}
| NUM {$$=NULL;}
;

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
;

stmt
: IF BOOL suite {$$=NULL;}
| matchedStmt {$$=NULL;}
;

matchedStmt
: IF bool suite ELSE suite {$$=NULL;}
| WHILE bool suite {$$=NULL;}
| varDecl ';' {$$=NULL;}
| return ';' {$$=NULL;}
;


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


