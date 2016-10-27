#include <stdio.h>
#include "symbol_table.h"


Class* build_ct(Program *p){
    Class *ct = NULL;
    Class *c, *tmp;

    if(p == NULL || c == NULL)
        return NULL;

    ClassDecl *cdecl = p->classes;
    while(cdecl != NULL){
        HASH_FIND_STR(ct, cdecl->selfName, tmp); // esta classe ja foi declarada?
        if(tmp == NULL){
            c = (Class*) malloc(sizeof(Class));
            c->selfName = strdup(cdecl->selfName);
            c->superName = strdup(cdecl->superName);
            c->line = cdecl->line;
            c->functions = NULL;
            c->fields = NULL;
            printf("Coloca %s na ct\n", c->selfName);


            HASH_ADD_KEYPTR(hh, ct, c->selfName, strlen(c->selfName), c);
        }else{
            printf("WARN %d: Class %s already declared at line %d, this declaration will be desconsidered\n", cdecl->line, tmp->selfName, tmp->line);
        }
        cdecl = cdecl->next;
    }

    return ct;
}

void print_ct(Class **ct){
   Class *tmp, *c = NULL;
   /*
   HASH_FIND_STR(ct, "a", c);
   printf("Achei %d:%s extd %s na ct\n", c->line, c->selfName, c->superName);
   HASH_FIND_STR(ct, "foo", c);
   printf("Achei %d:%s extd %s na ct\n", c->line, c->selfName, c->superName);
   HASH_FIND_STR(ct, "bar", c);
   printf("Achei %d:%s extd %s na ct\n", c->line, c->selfName, c->superName);
   */
   for(c=*ct; c != NULL; c = (c->hh.next))
       printf("Achei %d: %s extd %s na ct\n", c->line, c->selfName, c->superName);
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




