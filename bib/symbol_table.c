#include <stdio.h>
#include "symbol_table.h"


Class* build_ct(Program *p){
    Class *ct = NULL;
    Class *c = (Class*) malloc(sizeof(Class));

    if(p == NULL || c == NULL)
        return NULL;

    ClassDecl *cdecl = p->classes;
    c->selfName = cdecl->selfName;
    c->superName = c->superName;
    c->functions = NULL;
    c->fields = NULL;
    printf("Coloca %s na ct\n", c->selfName);

    HASH_ADD_KEYPTR(hh, ct, c->selfName,strlen(c->selfName), c);

    return ct;
}

void print_ct(Class *ct){
   Class *tmp, *c = NULL;
   HASH_ITER(hh, ct, c, tmp){
       printf("Achei %s na ct\n", c->selfName);
   }
}

/*
    ---------------------- # Hash Functions # -------------------------
*/
unsigned long hash_fun(unsigned char *str, int modulo){
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % modulo;
}


int ct_insert_class(Class **ct, Class *c){
    int try, hash;

    return 0;
}

int class_insert_fun(Function *fun, Class *cl); 
int class_insert_var(Variable *var, Class *cl); 
int function_insert_var(Variable *var, Function *fun); 




