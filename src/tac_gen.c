#include "tac_gen.h"

void gen_tac(Class *ct, Program *p){
    Class *main = NULL;
    HASH_FIND_STR(ct, "Object", main);
    if(main != NULL){
        printf(".table\n");
        Variable *v;
        for(v = main->fields; v != NULL; v = v->hh.next){
            if(!strcmp(v->type, "int") || !strcmp(v->type, "bool"))
                printf("int %s = 0", v->name);
        }
    }
}
