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
            c->selfName = cdecl->selfName;
            c->superName = cdecl->superName;
            c->line = cdecl->line;
            c->functions = NULL;
            c->fields = NULL;
            build_class_body(c, cdecl->cMembers);
            printf("Coloca %s na ct\n", c->selfName);


            HASH_ADD_KEYPTR(hh, ct, c->selfName, strlen(c->selfName), c);
        }else{
            printf("WARN %d: Class %s already declared at line %d, this declaration will be disconsidered\n", cdecl->line, tmp->selfName, tmp->line);
        }
        cdecl = cdecl->next;
    }

    return ct;
}

void build_class_body(Class *c, ClassMembers *cmem){
    Variable *v_table = NULL;
    while(cmem != NULL){
        if(cmem->member->utype == VAR_DECL)
            build_class_fields(cmem->member->member->varDecls, &v_table);
        cmem = cmem->next;
    }
    c->fields = v_table;

}

void build_class_fields(VarDecl *vars, Variable **v_table){
    Variable *v, *tmp;
    IdList *ids = vars->idList;
    while(ids != NULL){
        HASH_FIND_STR(*v_table, ids->id, tmp); // esta variavel ja foi declarada?
        if(tmp == NULL){
            v = (Variable*)malloc(sizeof(Variable));
            v->name = ids->id;
            v->type = vars->type;
            v->line = vars->line;
            v->ch_begin = ids->ch_begin;
            v->ch_end = ids->ch_end;
            HASH_ADD_KEYPTR(hh, *v_table, v->name, strlen(v->name), v);
        }else{
            printf("WARN %d-[%d-%d]: Variable %s already declared at line %d, this declaration will be disconsidered\n", vars->line, ids->ch_begin, ids->ch_end, ids->id, tmp->line);
        }
        ids = ids->next;
    }
    
}

void print_ct(Class **ct){
   Class *c = NULL;
   /*
   HASH_FIND_STR(ct, "a", c);
   printf("Achei %d:%s extd %s na ct\n", c->line, c->selfName, c->superName);
   */
   printf("------------------ # Class Table # ------------------\n");
   for(c=*ct; c != NULL; c = (c->hh.next)){
       printf("%d: %s extd %s{\n", c->line, c->selfName, c->superName);
       print_vars(c->fields);
       printf("}\n");
   }
}

void print_vars(Variable *vt){
    Variable *v;
    for(v=vt; v != NULL; v = (v->hh.next)){
        printf("\t%d: %s %s[%d-%d];\n", v->line, v->type, v->name, v->ch_begin, v->ch_end);
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





