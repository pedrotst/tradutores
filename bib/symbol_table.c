#include <stdio.h>
#include "symbol_table.h"


void print_varTable(VariableTable *varTable){
    printf("### --- Variable Table --- ###\n");
    printf("names \t\ttypes\n");
    printf("-----------------------------\n");
    while(varTable != NULL){
        printf("%s \t\t%s\n", varTable->name, varTable->type);
        varTable = varTable->next;
    }
    printf("\n");
}

void print_funTable(FunctionTable *ftable){
    printf("### --- Function Table --- ###\n");
    printf("-----------------------------\n");
    while(ftable != NULL){
        printf("%s %s(", ftable->type, ftable->name);
        print_fargs(ftable->fargs);
        printf(")\n");
        ftable = ftable->next;
    }
    printf("\n");
}

void print_classTable(ClassTable *ctable){
    printf("### --- Class Table --- ###\n");
    printf("-----------------------------\n");
    while(ctable != NULL){
        printf("class %s extends %s\n", ctable->selfName, ctable->superName);
        ctable = ctable->next;
    }
    printf("\n");
}
