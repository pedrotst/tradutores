#include "tac_gen.h"

void gen_tac(Class *ct, Program *p){
    Class *obj = NULL;
    Function *main = NULL;
    HASH_FIND_STR(ct, "Object", obj);
    HASH_FIND_STR(obj->functions, "main", main);

    if(main != NULL){
        printf(".table\n");
        Variable *v;
        for(v = main->vars; v != NULL; v = v->hh.next){
            if(!strcmp(v->type, "int") || !strcmp(v->type, "bool"))
                printf("int %s = 0\n", v->name);
        }
    }
}
