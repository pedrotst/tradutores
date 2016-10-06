#ifndef __sTable_h
#define __sTable_h

typedef enum SymbolType_e{
    VAR, FUN
}SymbolType;

typedef struct SymbolTable_s{
    char *Id;
    SymbolType type;
    struct SymbolTable_s *next;
}SymbolTable;

#endif
