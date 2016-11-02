#ifndef __sTable_h
#define __sTable_h
#include "uthash.h"
#include "debug.h"

typedef enum SymbolType_e{
    VAR, FUN
}SymbolType;

// a main sera armazenada numa classe chamada Object
typedef struct ClassTable_s ClassTable;
typedef struct Class_s Class;
typedef struct Function_s Function; 
typedef struct Variable_s Variable;
typedef struct Type_s Type;
#include "ast.h"

extern Class *ct;
extern char *file_name;

struct Variable_s{
    char *name, *type;
    Class *tref;
    int line, ch_begin, ch_end;
    UT_hash_handle hh;
};

struct Function_s{
    char *name, *type;
    Class *tref;
    int line, chbegin;
    Variable *vars;
    StmtList *stmts;
    FormalArgs *fargs;
    Class *this;
    UT_hash_handle hh;
};

struct Class_s{
    char *selfName;
    char *superName;
    struct Class_s *super;
    int line;
    Function *functions;
    Variable *fields;
    UT_hash_handle hh;
};

// ClassTable* classTable_insert(char *className, char *superName, 
/* Hash Functions */
// int ct_insert_class(Class **ctable, Class *ct);

void class_insert_function(Function *fun, Class *cl); 
void class_insert_var(Variable *var, Class *cl); 
int function_insert_var(Variable *var, Function *fun); 
Class* resolve_type(char *type);
void check_stmts(StmtList *stmts, Function *f);
void check_bool(Exp *e, Function *f);

void add_Object_ct();
void build_ct(Program *p);
void hash_insert_fargs(FormalArgs *fargs, Variable **v_table);
void hash_insert_varDecl(VarDecl *vars, Variable **v_table);
void hash_insert_function(FunctionDecl *funs, Function **f_table, Class *c);
void insert_class_functions(ConstrDecl *constrs, Function **f_table);
void build_class_body(Class *c, ClassMember *cmem);


void print_ct();
void print_vars(Variable *vt, int tabs);
void print_functions(Function *ft);

void print_arq_line(int line, int ch_begin, int ch_end);
#endif
