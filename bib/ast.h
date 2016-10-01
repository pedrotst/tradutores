#ifndef __ast_h
#define __ast_h

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define TERM_T 1
#define EXP_T 2
#define LINE_T 3
#define OPNODE_T 4


typedef struct Program_s        Program;
typedef struct ClassDecl_s      ClassDecl;
typedef struct Constructor_s    Constructor;
typedef struct MethodDecl_s     MethodDecl;
typedef struct ClassName_s      ClassName;
typedef struct Assignment_s     Assignment;
typedef struct Var_s            Var;
typedef struct Suite_s          Suite;
typedef struct StmtList_s       StmtList;
typedef struct ArgList_s        ArgList;
typedef struct FormalArgs_s     FormalArgs;
typedef struct VarDecl_s        VarDecl;
typedef struct IdList_s         IdList;
typedef struct Type_s           Type;
typedef struct Exp_s            Exp;
typedef struct FieldAccess_s    FieldAccess;
typedef struct MethodInvoc_s    MethodInvoc;
typedef struct New_s            New;
typedef struct Int_s            Int;
typedef struct Bool_s           Bool;
typedef struct Stmt_s           Stmt;
typedef struct MatchedStmt_s    MatchedStmt;

struct ClassDecl_s{
    char *className;
    char *superName;
    VarDecl *varDecls;
    Constructor *construct;
    MethodDecl *mdecl;
    struct ClassDecl_s *nextClass;
};


struct Program_s{
    ClassDecl *classes;
    StmtList *stmts;
};


Program* program_node(ClassDecl *classes, StmtList *stmts);

/*
typedef struct exp_s exp;

typedef union symbol_un {
    char op;
    int num;
}symbol_u;

typedef struct symbol_s{
    int tag;
    symbol_u u;
}symbol;

typedef struct term_s{
    int num;
}term;

typedef struct bin_op_s{
    char op;
    exp *lhs;
    term *rhs;
}bin_op;

union exp_u{
    term *t;
    bin_op *b_op;
};

struct exp_s{
    int tag;
    union exp_u expr;
};
exp* exp_alloc();
term* term_alloc();
bin_op* bin_alloc();
term* term_node(int n);

exp* exp_opnode(char op, exp* lhs, term* rhs);
exp* exp_termnode(term* t);
void print_exp(exp* a, int n);
void print_opnode(bin_op* b_op, int tabs);
void print_term(term* t, int tabs);

void destruct_tree(exp* a);
void print_symbol(symbol s);
void print_ntype(int n_type);
*/

#endif
