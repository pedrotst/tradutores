#ifndef __ast_h
#define __ast_h

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

/*tags serão utilizadas para tipagem das unions*/
typedef enum type_enum{
    VAR_DECL, FUN_DECL
}tag;


typedef struct Program_s            Program;
typedef struct ClassDecl_s          ClassDecl;
typedef struct FunctionDecl_s       FunctionDecl;
typedef struct FunctionList_s       FunctionList;
typedef struct ConstructorList_s    ConstructorList;
//typedef struct Constructor_s      Constructor;
//typedef struct MethodDecl_s       MethodDecl;
typedef struct ClassMembers_s       ClassMembers;
//typedef struct ClassName_s        ClassName;
typedef struct Assignment_s         Assignment;
typedef struct Var_s                Var;
typedef struct Suite_s              Suite;
typedef struct StmtList_s           StmtList;
typedef struct ArgList_s            ArgList;
typedef struct FormalArgs_s         FormalArgs;
typedef struct VarDecl_s            VarDecl;
typedef struct IdList_s             IdList;
typedef struct Exp_s                Exp;
typedef struct Object_s             Object;
typedef struct IdList_s             IdList;
typedef struct Return_s             Return;
typedef struct FieldAccess_s        FieldAccess;
typedef struct MethodInvoc_s        MethodInvoc;
typedef struct IdList_s             IdList;
typedef struct New_s                New;
typedef struct Int_s                Int;
typedef struct Bool_s               Bool;
typedef struct Stmt_s               Stmt;
typedef struct MatchedStmt_s        MatchedStmt;

struct ClassDecl_s{
    char *selfName;
    char *superName;
    ClassMembers *cMembers;
    struct ClassDecl_s *nextClass;
};

struct IdList_s{
    char *id;
    IdList *nextId;
};

struct VarDecl_s{
    char *type;
    char *id;
    IdList *idList;
};

struct FunctionDecl_s{
    int isConstructor;
    char *name;
    char *type;
    FormalArgs *fargs;
    StmtList *stmts;
};

union FunctionDecl_u{
    FunctionList *fun;
    ConstructorList *constr;
};

typedef union ClassMembers__u{
    VarDecl *varDecls;
    FunctionDecl *funDecl;
}ClassMembers_u;

struct ClassMembers_s{
    tag utype;
    ClassMembers_u *member;
    struct ClassMembers_s *nextMember;
};

struct Program_s{
    ClassDecl *classes;
    StmtList *stmts;
};


Program* program_node(ClassDecl *classes, StmtList *stmts);

ClassDecl* classDecl_node(char *Selfname, char *superName, 
    ClassMembers *cMembers, ClassDecl *nextClass);

ClassMembers* classMember_node(tag utype, VarDecl *varDecls, 
    FunctionDecl *funDecls, 
    ClassMembers *nextMember);

VarDecl* varDecl_node(char *type, char *id, IdList *ids);

IdList* idList_node(char *id, IdList *ids);

FunctionDecl* functionDecl_node(int isConstructor, char *name, char *type,
    FormalArgs *fargs, StmtList *stmtList);

void print_program(Program* p);
void print_class(ClassDecl *c);
void print_stmt(StmtList *stmt);

void print_classMembers(ClassMembers *cmember);
void print_idList(IdList *ids);
void print_varDecl(VarDecl *varDecls);
void print_funDecl(FunctionDecl *funDecl);
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
