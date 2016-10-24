#ifndef __sTable_h
#define __sTable_h


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
};

struct FunctionTable_s{
    char *name;
    char *type;
    int line, chbegin;
    int var_cur_size, var_el_num;
    Variable **vars;
    StmtList *stmts;
};

/* 
ClassTable é uma hashtable de tamanho variável,
ela possúi um tamanho atual para, caso ela lote faremos realoc de 2*cur_size, ela é indexada no nome da classe (i.e. *selfName)
*/
struct ClassTable_s{
    int cur_size, el_num;
    Class **classes;
};

struct Class_s{
    char *selfName;
    char *superName;
    int line, ch_begin, ch_end;
    int fun_cur_size, fun_el_num;
    Function **functions;
    int fields_cur_size, fields_el_num;
    Variable **fields;
};

// ClassTable* classTable_insert(char *className, char *superName, 
/* Hash Functions */
unsigned long hash_fun(unsigned char *str, int modulo);
int new_ct(ClassTable **ct);
int ct_insert_class(Class *cl, ClassTable *ct);
int class_insert_fun(Function *fun, Class *cl); 
int class_insert_var(Variable *var, Class *cl); 
int function_insert_var(Variable *var, Function *fun); 

#endif
