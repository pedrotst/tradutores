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

struct FunctionTable_s{
    char *name;
    char *type;
    int line, chbegin;
    Variable *vars;
    StmtList *stmts;
    UT_hash_handle hh;
};

struct Class_s{
    char *selfName;
    char *superName;
    int line;
    Function *functions;
    Variable *fields;
    UT_hash_handle hh;
};

// ClassTable* classTable_insert(char *className, char *superName, 
/* Hash Functions */
int ct_insert_class(Class **ctable, Class *ct);
int class_insert_fun(Function *fun, Class *cl); 
int class_insert_var(Variable *var, Class *cl); 
int function_insert_var(Variable *var, Function *fun); 

Class* build_ct(Program *p);
void build_class_fields(VarDecl *vars, Variable **v_table);
void build_class_body(Class *c, ClassMember *cmem);
void print_ct(Class **ctable);
void print_vars(Variable *vt);

#endif
