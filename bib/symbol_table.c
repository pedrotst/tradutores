#include <stdio.h>
#include "symbol_table.h"


/*
    ---------------------- # Hash Functions # -------------------------
*/
unsigned long hash_fun(unsigned char *str){
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

int ct_insert_class(Class *cl, ClassTable *ct){
    int try, hash;
    if(ct->cur_size >= ct->el_num){
        int new_size = 2 * ct->cur_size;
        ct->classes = (Class**)realloc(ct, new_size * sizeof(Class*));
        ct->cur_size = new_size;
    }

    hash = hash_fun(cl->selfName);
    if(ct->classes[hash] == 0) { // empty cell
        ct->classes[hash] = cl;
        ct->el_num++;
        return 1;
    }

    return 0;
}
int class_insert_fun(Function *fun, Class *cl); 
int class_insert_var(Variable *var, Class *cl); 
int function_insert_var(Variable *var, Function *fun); 




