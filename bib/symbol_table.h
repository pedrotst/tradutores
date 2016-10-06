#ifndef __sTable_h
#define __sTable_h


typedef enum SymbolType_e{
    VAR, FUN
}SymbolType;

// a main sera armazenada numa classe chamada main
typedef struct ClassTable_s ClassTable;
typedef struct FunctionTable_s FunctionTable; 
typedef struct VariableTable_s VariableTable;
#include "ast.h"


struct VariableTable_s{
    char *name;
    char *type;
    struct VariableTable_s *next;
};

struct FunctionTable_s{
    char *name;
    char *type;
    FormalArgs *fargs;
    struct FunctionTable_s *next;
};

struct ClassTable_s{
    char *selfName;
    char *superName;
    FunctionTable *functions;
    VariableTable *variables;
    struct ClassTable_s *next;
};

// ClassTable* classTable_insert(char *className, char *superName, 
void print_varTable(VariableTable *varTable);
void print_funTable(FunctionTable *ftable);
void print_classTable(ClassTable *ctable);

#endif
