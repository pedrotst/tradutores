#include <stdio.h>
#include "symbol_table.h"


unsigned long hash_fun(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

/*
    ---------------------- # Print Tables # -------------------------
*/
void print_varTable(VariableTable *varTable){
    printf("### --- Variable Table --- ###\n");
    printf("name\t\ttype\t\tline\t\tcollum\n");
    printf("---------------------------------------------------------------------------\n");
    while(varTable != NULL){
        printf("%s\t\t%s\t\t%d\t\t%d\n", varTable->name, varTable->type,varTable->line, varTable->chbegin);
        varTable = varTable->next;
    }
    printf("\n");
}

void print_funTable(FunctionTable *ftable){
    printf("### --- Function Table --- ###\n");
    printf("-----------------------------\n");
    while(ftable != NULL){
        printf("line %d,%d: %s %s(", ftable->line, ftable->chbegin,ftable->type, ftable->name);
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
        printf("line %d,%d: class %s extends %s\n", ctable->line, ctable->chbegin,ctable->selfName, ctable->superName);
        ctable = ctable->next;
    }
    printf("\n");
}

/*
    ----------------------------- # Destruct Tables # ---------------------
*/

void destruct_varTable(VariableTable *vt){
    if(vt != NULL){
        destruct_varTable(vt->next);
        free(vt);
    }
}

void destruct_funTable(FunctionTable *ft){
    if(ft != NULL){
        destruct_funTable(ft->next);
        free(ft);
    }
}

void destruct_classTable(ClassTable *ct){
    if(ct != NULL){
        destruct_funTable(ct->functions);
        destruct_varTable(ct->variables);
        destruct_classTable(ct->next);
        free(ct);
    }
}
