#ifndef __sTable_h
#define __sTable_h
#include "uthash.h"

typedef enum SymbolType_e{
    VAR, FUN
}SymbolType;

// a main sera armazenada numa classe chamada Object
typedef struct ClassTable_s ClassTable;
typedef struct Class_s Class;
typedef struct Function_s Function; 
typedef struct Variable_s Variable;
#include "ast.h"


struct Variable_s{
    char *name;
    char *type;
    int line, ch_begin, ch_end;
    UT_hash_handle hh;
};

struct Function_s{
    char *name;
    char *type;
    int line, chbegin;
    Variable *vars;
    StmtList *stmts;
    FormalArgs *fargs;
    Class *this;
    UT_hash_handle hh;
};

struct Class_s{
    char *selfName;
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

Class* build_ct(Program *p);
void hash_insert_variable(VarDecl *vars, Variable **v_table);
void hash_insert_function(FunctionDecl *funs, Function **f_table);
void insert_class_functions(ConstrDecl *constrs, Function **f_table);
void build_class_body(Class *c, ClassMember *cmem);
void print_ct(Class **ctable);
void print_vars(Variable *vt);
void print_functions(Function *ft);

#endif
