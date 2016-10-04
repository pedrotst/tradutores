#ifndef __ast_h
#define __ast_h

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

/*tags serão utilizadas para tipagem das unions*/
typedef enum type_enum{
    VAR_DECL, FUN_DECL, CONSTR_DECL,
    IF_STMT, RET_STMT, WHILE_STMT
}tag;


typedef struct Program_s            Program;
typedef struct ClassDecl_s          ClassDecl;
typedef struct FunctionDecl_s       FunctionDecl;
//typedef struct MethodDecl_s       MethodDecl;
typedef struct ClassMembers_s       ClassMembers;
typedef struct ClassMember_s        ClassMember;
typedef struct ConstrDecl_s         ConstrDecl;
typedef struct FunctionDecl_s       FunctionDecl;
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
typedef struct Stmt_s                Stmt;
typedef struct IfStmt_s             IfStmt;
typedef struct WhileStmt_s          WhileStmt;
typedef struct ReturnStmt_s         ReturnStmt;

struct ClassDecl_s{
    char *selfName;
    char *superName;
    ClassMembers *cMembers;
    struct ClassDecl_s *next;
};

struct IfStmt_s {
    Exp *cond;
    StmtList *then, *els; // when els != NULL we have and else suite
};

typedef union Stmt__u{
    IfStmt *ifStmt;
    WhileStmt *whileStmt;
    Exp *returnExp;
    VarDecl *varDecl;
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
    IdList *next;
};

struct VarDecl_s{
    char *type;
    char *id;
    IdList *idList;
};

struct FormalArgs_s{
    char *type;
    char *name;
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
    FormalArgs *fargs;
    StmtList *stmts;
};

typedef union ClassMember__u{
    VarDecl *varDecls;
    FunctionDecl *funDecl;
    ConstrDecl *constrDecl;
}ClassMember_u;

struct ClassMembers_s{
    ClassMember *member;
    ClassMembers *next;
};

struct ClassMember_s{
    tag utype;
    ClassMember_u *member;
};

struct Program_s{
    ClassDecl *classes;
    StmtList *stmts;
};


Program* program_node(ClassDecl *classes, StmtList *stmts);

ClassDecl* classDecl_node(char *Selfname, char *superName, 
    ClassMembers *cMembers, ClassDecl *next);

ClassMembers* classMembers_node(ClassMember *member,
    ClassMembers *head);

ClassMember* classMember_node(tag utype, VarDecl *varDecl, 
    FunctionDecl *funDecl, ConstrDecl *constrDecl);

VarDecl* varDecl_node(char *type, char *id, IdList *ids);

IdList* idList_node(char *id, IdList *head);

ConstrDecl* constrDecl_node(char *name,
    FormalArgs *fargs, StmtList *stmtList);

FunctionDecl* functionDecl_node(char *type, char *name,
    FormalArgs *fargs, StmtList *stmtList);
FormalArgs* formalArgs_node(char *type, char *name, FormalArgs *head);

StmtList* stmtList_node(Stmt *stmt, StmtList *head);

Stmt* stmt_node(tag utype, VarDecl *varDecl, IfStmt *ifStmt,
    WhileStmt *whileStmt, Exp *returnStmt);

IfStmt* if_node(Exp *cond, StmtList *then, StmtList *els);

void print_program(Program* p);
void print_class(ClassDecl *c);
void print_stmt(StmtList *stmt, int tabs);

void print_classMembers(ClassMembers *cmember);
void print_idList(IdList *ids);
void print_fargs(FormalArgs *fargs);
void print_varDecl(VarDecl *varDecls, int tabs);
void print_funDecl(FunctionDecl *funDecl);
void print_constrDecl(ConstrDecl *constrDecl);
void print_return(Exp *e, int tabs);


#endif
