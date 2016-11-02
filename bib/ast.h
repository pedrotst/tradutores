#ifndef __ast_h
#define __ast_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
/*tags serão utilizadas para tipagem das unions*/
typedef enum type_enum{
    VAR_DECL, FUN_DECL, CONSTR_DECL,
    IF_STMT, RET_STMT, WHILE_STMT, ASSGN_STMT,
    ID_VAR, OBJ_VAR,
    FIELD_OBJ, METH_OBJ, NEW_OBJ,
    VAR_EXP, BINOP_EXP, PAR_EXP, PRIM_EXP,
    BOOL_PRIM, INT_PRIM,
    BASE_TYPE, CLASS_TYPE
}tag;


typedef struct Program_s            Program;
typedef struct ClassDecl_s          ClassDecl;
typedef struct FunctionDecl_s       FunctionDecl;
typedef struct ClassMember_s        ClassMember;
typedef struct ConstrDecl_s         ConstrDecl;
typedef struct Assignment_s         Assignment;
typedef struct Var_s                Var;
typedef struct Suite_s              Suite;
typedef struct StmtList_s           StmtList;
typedef struct ArgList_s            ArgList;
typedef struct FormalArgs_s         FormalArgs;
typedef struct VarDecl_s            VarDecl;
typedef struct Exp_s                Exp;
typedef struct BinOp_s              BinOp;
typedef struct Object_s             Object;
typedef struct Return_s             Return;
typedef struct FieldAccess_s        FieldAccess;
typedef struct MethodInvoc_s        MethodInvoc;
typedef struct IdList_s             IdList;
typedef struct New_s                New;
typedef struct Int_s                Int;
typedef struct Bool_s               Bool;
typedef struct Stmt_s                Stmt;
typedef struct IfStmt_s             IfStmt;
typedef struct WhileStmt_s          WhileStmt;
typedef struct ReturnStmt_s         ReturnStmt;
typedef struct Primary_s         Primary;

#include "symbol_table.h"

struct ClassDecl_s{
    char *selfName;
    char *superName;
    int line;
    ClassMember *cMembers;
    struct ClassDecl_s *next;
};

struct IfStmt_s {
    Exp *cond;
    StmtList *then, *els; // when els != NULL we have and else suite
};

struct WhileStmt_s {
    Exp *cond;
    StmtList *loop;
};

struct BinOp_s{
    char op;
    Exp *lhs, *rhs;
};

typedef union Exp__u{
    Var *var;
    BinOp *binOp;
    Exp *parenthesis;
    Primary *primary;
}Exp_u;

struct Primary_s{
    tag type;
    int val;
};

struct Exp_s{
    tag utype;
    Exp_u *exp_u;
};

typedef union Object__u{
    FieldAccess *field;
    MethodInvoc *meth;
    New *newObj;
}Object_u;

struct Object_s{
    tag utype;
    Object_u *obj_u;
};

struct ArgList_s{
    Exp *arg;
    struct ArgList_s *next;
};

struct MethodInvoc_s{
    Var *obj; 
    char *mname;
    ArgList *args;
};

struct FieldAccess_s{
    Var *obj;
    char *fname;
};

struct New_s{
    char *cname;
    ArgList *args;
};

typedef union Var__u{
    char *id;
    Object *obj;
}Var_u;

struct Var_s{
    tag utype;
    Var_u *var_u;
};

struct Assignment_s{
    Var *lhs;
    Exp *rhs;
};

typedef union Stmt__u{
    IfStmt *ifStmt;
    WhileStmt *whileStmt;
    Exp *returnExp;
    VarDecl *varDecl;
    Assignment *assgn;
}Stmt_u;

struct Stmt_s{
    tag utype;
    Stmt_u *stmt_u;
};

struct StmtList_s{
    Stmt *stmt;
    struct StmtList_s *next;
};


struct IdList_s{
    char *id;
    int ch_begin, ch_end;
    IdList *next;
};

struct VarDecl_s{
    char *type;
    int line;
    IdList *idList;
};

struct FormalArgs_s{
    char *type, *name;
    int line, ch_end, ch_begin;
    struct FormalArgs_s *next;
};

struct ConstrDecl_s{
    char *name;
    FormalArgs *fargs;
    StmtList *stmts;
};

struct FunctionDecl_s{
    char *name;
    char *type;
    int line;
    FormalArgs *fargs;
    StmtList *stmts;
};

typedef union ClassMember__u{
    VarDecl *varDecls;
    FunctionDecl *funDecl;
    ConstrDecl *constrDecl;
}ClassMember_u;

struct ClassMember_s{
    tag utype;
    ClassMember_u *member;
    ClassMember *next;
};

struct Program_s{
    ClassDecl *classes;
    StmtList *stmts;
};




Program* program_node(ClassDecl *classes, StmtList *stmts);

ClassDecl* classDecl_node(char *Selfname, char *superName,
    ClassMember *cMembers, ClassDecl *next,
    int line);

ClassMember* classMembers_node(ClassMember *member,
    ClassMember *head);

ClassMember* classMember_node(tag utype, VarDecl *varDecl,
    FunctionDecl *funDecl, ConstrDecl *constrDecl);

VarDecl* varDecl_node(char *type, IdList *ids,
    int line, int chbegin);

IdList* idList_node(char *id, int ch_begin, int ch_end, IdList *head);

ConstrDecl* constrDecl_node(char *name,
    FormalArgs *fargs, StmtList *stmtList);

FunctionDecl* functionDecl_node(char *type, char *name,
    FormalArgs *fargs, StmtList *stmtList, int line);

FormalArgs* formalArgs_node(char *type, char *name, FormalArgs *head, int line, int ch_begin, int ch_end);

StmtList* stmtList_node(Stmt *stmt, StmtList *head);

Stmt* stmt_node(tag utype, VarDecl *varDecl, IfStmt *ifStmt,
    WhileStmt *whileStmt, Exp *returnStmt, Assignment *assgn);

IfStmt* if_node(Exp *cond, StmtList *then, StmtList *els);
WhileStmt* while_node(Exp *cond, StmtList *loop);

Assignment* assignment_node(Var *lhs, Exp *rhs);

Object* object_node(tag utype, FieldAccess *field,
    MethodInvoc *meth, New *newObj);

Var* var_node(tag utype, char *id, Object *obj);
Exp* exp_node(tag utype, Var *var, BinOp *binOp,
    Exp *parenthesis, Primary *primary);

MethodInvoc* methodInvoc_node(Var *obj, char *mname, ArgList *args);
FieldAccess* fieldAccess_node(Var *obj, char *fname);
New* new_node(char *cname, ArgList *args);

ArgList* argList_node(Exp *arg, ArgList *head);

BinOp* binOp_node(char op, Exp *lhs, Exp *rhs);
Primary* primary_node(tag type, int val);

void print_program(Program* p);
void print_class(ClassDecl *c);
void print_stmt(StmtList *stmt, int tabs);
void print_classMembers(ClassMember *cmember);
void print_idList(IdList *ids);
void print_fargs(FormalArgs *fargs);
void print_varDecl(VarDecl *varDecls, int tabs);
void print_funDecl(FunctionDecl *funDecl);
void print_constrDecl(ConstrDecl *constrDecl);
void print_return(Exp *e, int tabs);
void print_var(Var *v);
void print_if(IfStmt *i, int tabs);
void print_while(WhileStmt *w, int tabs);
void print_exp(Exp *e);
void print_binOp(BinOp *binOp);
void print_int(Int *i);
void print_bool(Bool *b);
void print_assignment(Assignment *assgn, int tabs);
void print_obj(Object *obj);
void print_methodInvoc(MethodInvoc *minvok);
void print_fieldAccess(FieldAccess *faccess);
void print_new(New *n);
void print_prim(Primary *prim);
void print_argList(ArgList *args);
void print_tabs(int tabs);

void destruct_program(Program *p);
void destruct_classDecl(ClassDecl *cDecl);
void destruct_stmtList(StmtList *sl);
void destruct_stmt(Stmt *stmt);
void destruct_classMembers(ClassMember *cMems);
void destruct_classMember(ClassMember *cMem);
void destruct_varDecl(VarDecl *vars);
void destruct_functionDecl(FunctionDecl *funs);
void destruct_constrDecl(ConstrDecl *constrs);
void destruct_idList(IdList *ids);
void destruct_formalArgs(FormalArgs *fargs);
void destruct_ifStmt(IfStmt *ifStmt);
void destruct_whileStmt(WhileStmt *whileStmt);
void destruct_exp(Exp *e);
void destruct_varDecl(VarDecl *vDecl);
void destruct_assignment(Assignment *assgn);
void destruct_var(Var *v);
void destruct_object(Object *obj);
void destruct_fieldAccess(FieldAccess *fAccess);
void destruct_methodInvoc(MethodInvoc *mInvok);
void destruct_new(New *new);
void destruct_binOp(BinOp *op);
void destruct_primary(Primary *prim);
void destruct_argList(ArgList *args);

#endif
