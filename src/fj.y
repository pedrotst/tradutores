%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>
#include "bib/ast.h"
#include "bib/symbol_table.h"

int count_lines = 1, ch_end= 0, ch_begin = 0;

void yyerror(const char*);
int yywrap();
int yylex(void);

// Global Vars
Program *p; 
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
    struct BinOp_s*              binOp_v;
    struct Assignment_s*    assignment_v; 
    struct Var_s*           var_v; 
    struct StmtList_s*      stmtList_v;
    struct ArgList_s*       argList_v; 
    struct FormalArgs_s*    formalArgs_v; 
    struct VarDecl_s*       varDecl_v; 
    struct IdList_s*        idList_v; 
    struct Exp_s*           exp_v; 
    struct FieldAccess_s*   fieldAccess_v; 
    struct MethodInvoc_s*   methodInvoc_v; 
    struct New_s*           new_v; 
    struct Int_s*           int_v; 
    struct Bool_s*          bool_v; 
    struct Stmt_s*          stmt_v; 
    struct Object_s*        object_v; 
    struct Primary_s*       prim_v;

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
%right '='

%type <num> NUM TRUE FALSE 
%type <prim_v> primary
%type <strs> ID INT BOOL type
%type <program_v> program 
%type <class_v> classDecl
%type <classMembers_v> classMembers
%type <classMember_v> classMember
%type <constrDecl_v> constrDecl
%type <functionDecl_v> functionDecl
%type <assignment_v> assignment
%type <var_v> var 
%type <stmtList_v>stmtList suite
%type <argList_v> argList
%type <formalArgs_v> formalArgs
%type <varDecl_v> varDecl
%type <idList_v> idList
%type <exp_v> exp 
%type <binOp_v> binOp NOT
%type <object_v> object
%type <fieldAccess_v> fieldAccess
%type <methodInvoc_v> methodInvoc
%type <new_v> new
%type <stmt_v> stmt
%nonassoc IF
%nonassoc ELSE

%%
program 
: classDecl stmtList {
    p = program_node($1, $2); 
    $$ = p;
}    
;

classDecl
: %empty {$$ = NULL;}
| classDecl CLASS ID EXTENDS ID '{' classMembers '}' {
    $$ = classDecl_node($3, $5, $7, $1, count_lines, ch_end);
}
| classDecl CLASS ID EXTENDS ID '{' '}' {
    $$ = classDecl_node($3, $5, NULL, $1, count_lines, ch_end);
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
: type ID '(' formalArgs ')' '{' stmtList '}' {$$ = functionDecl_node ($1, $2, $4, $7, count_lines, ch_end);}
;

stmtList
: %empty {$$ = NULL;}
| stmtList stmt {$$ = stmtList_node($2, $1);}
;

argList
: %empty {$$=NULL;}
| exp {$$ = argList_node($1, NULL);}
| argList ',' exp {$$=argList_node($3, $1);}
;

formalArgs
: %empty {$$=NULL;}
| type ID {$$=formalArgs_node($1, $2, NULL);}
| formalArgs ',' type ID {$$=formalArgs_node($3, $4, $1);}
;

varDecl
: type ID idList {
    IdList *ids = idList_node($2, ch_end, ch_begin, $3);
    $$ = varDecl_node($1, ids, count_lines, ch_end);
}
;

idList
: %empty {$$= NULL;}
| idList ',' ID {$$=idList_node($3, ch_end, ch_begin, $1);}
;


type
: ID {$$=$1;}
| INT {$$=$1;}
| BOOL {$$=$1;}
;

exp
: var {$$=exp_node(VAR_EXP, $1, NULL, NULL, NULL);}
| binOp {$$=exp_node(BINOP_EXP, NULL, $1, NULL, NULL);}
| '(' exp ')' {$$=exp_node(PAR_EXP, NULL, NULL, $2, NULL);}
| primary {$$ = exp_node(PRIM_EXP, NULL, NULL, NULL, $1);}

assignment
: var '=' exp {$$=assignment_node($1, $3);}
;

var
: ID {$$=var_node(ID_VAR, $1, NULL);}
| object {$$=var_node(OBJ_VAR, NULL, $1);}


object
: fieldAccess {$$=object_node(FIELD_OBJ, $1, NULL, NULL);}
| methodInvoc {$$=object_node(METH_OBJ, NULL, $1, NULL);}
| new {$$=object_node(NEW_OBJ, NULL, NULL, $1);}


methodInvoc
: var DOT ID '(' argList ')' {$$=methodInvoc_node($1, $3, $5);}
| ID '(' argList ')' {$$=methodInvoc_node(NULL, $1, $3);}
;

fieldAccess
: var DOT ID {$$=fieldAccess_node($1, $3);}
;

new
: NEW ID '(' argList ')' {$$=new_node($2, $4);}
;

binOp
: exp '+' exp {$$=binOp_node('+', $1, $3);}
| exp '-' exp {$$=binOp_node('-', $1, $3);}
| exp '*' exp {$$=binOp_node('*', $1, $3);}
| exp '/' exp {$$=binOp_node('/', $1, $3);}
| NOT exp {$$=binOp_node('!', $2, NULL);}
| exp BOR exp {$$=binOp_node('|', $1, $3);}
| exp BAND exp{$$=binOp_node('&', $1, $3);}
| exp BEQ exp {$$=binOp_node('=', $1, $3);}
| exp BLE exp {$$=binOp_node('(', $1, $3);}
| exp BGE exp {$$=binOp_node(')', $1, $3);}
| exp BLT exp {$$=binOp_node('<', $1, $3);}
| exp BGT exp {$$=binOp_node('>', $1, $3);}

primary
: TRUE { $$ = primary_node(BOOL_PRIM, 1);}
| FALSE{$$ = primary_node(BOOL_PRIM, 0);}
| NUM {$$ = primary_node(INT_PRIM, $1);}
;

stmt
: IF exp suite {
    IfStmt *a = if_node($2, $3, NULL);
    $$=stmt_node(IF_STMT, NULL, a, NULL, NULL, NULL);
    }
| IF exp suite ELSE suite %prec ELSE{
    IfStmt *a = if_node($2, $3, $5);
    $$=stmt_node(IF_STMT, NULL, a, NULL, NULL, NULL);
    }
| WHILE exp suite {
    WhileStmt *w = while_node($2, $3);
    $$=stmt_node(WHILE_STMT, NULL, NULL, w, NULL, NULL);}
| varDecl ';' { $$=stmt_node(VAR_DECL, $1, NULL, NULL, NULL, NULL); }
| assignment ';' {$$ = stmt_node(ASSGN_STMT, NULL, NULL, NULL, NULL, $1);}
| RETURN exp ';' {$$=stmt_node(RET_STMT, NULL, NULL, NULL, $2, NULL);}
| error ';' {yyerrok;$$ = NULL;}
;

suite
: '{' stmtList '}' {$$=$2;}
| stmt { $$=stmtList_node($1, NULL);}
;


%%


void yyerror(const char *str)
{
    fprintf(stderr,"Error line %d:%d: %s\n",count_lines, ch_end, str);
}

int yywrap() {return 1;};


int main()
{
    yyparse();
    if(yynerrs == 0){ // So printa se o parse foi ok
        print_program(p);
    }

    printf("%lu\n", hash_fun("a", 1000) );
    printf("%lu\n", hash_fun("b", 1000) );
    printf("%lu\n", hash_fun("c", 1000) );
    printf("%lu\n", hash_fun("pedro", 1000) );
    printf("%lu\n", hash_fun("Pedro", 1000) );
    printf("%lu\n", hash_fun("Marcos", 1000) );
    printf("%lu\n", hash_fun("Object", 1000) );
    printf("%lu\n", hash_fun("System", 1000) );

    destruct_program(p);
    return 0;
}


