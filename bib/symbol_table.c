#include <stdio.h>
#include "symbol_table.h"


/*
    ---------------------- # CT Building Functions # -------------------------
*/

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

void build_class_body(Class *c, ClassMember *cmem){
    Variable *v_table = NULL;
    Function *f_table = NULL;
    while(cmem != NULL){
        if(cmem->utype == VAR_DECL)
            hash_insert_variable(cmem->member->varDecls, &v_table);
        else if(cmem->utype == FUN_DECL)
            hash_insert_function(cmem->member->funDecl, &f_table);

        cmem = cmem->next;
    }
    c->fields = v_table;
    c->functions = f_table;

}

void hash_insert_variable(VarDecl *vars, Variable **v_table){
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


void hash_insert_function(FunctionDecl *funs, Function **f_table){
    Function *f;
    HASH_FIND_STR(*f_table, funs->name, f);
    if(f == NULL){
        f = (Function*)malloc(sizeof(Function));
        f->name = funs->name;
        f->type = funs->type;
        f->line = funs->line;
        f->vars = NULL;
        f->stmts = funs->stmts;
        HASH_ADD_KEYPTR(hh, *f_table, f->name, strlen(f->name), f);
    }
    else{
            printf("WARN %d: Function %s already declared at line %d, this declaration will be disconsidered\n", funs->line, funs->name, f->line);
    }
}


/*
    ---------------------- # Print Functions # -------------------------
*/

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
       print_functions(c->functions);
       printf("}\n");
   }
}

void print_vars(Variable *vt){
    Variable *v;
    for(v=vt; v != NULL; v = (v->hh.next)){
        printf("\t%d: %s %s[%d-%d];\n", v->line, v->type, v->name, v->ch_begin, v->ch_end);
    }
}


void print_functions(Function *ft){
    Function *f;
    for(f=ft; f != NULL; f = (f->hh.next)){
        printf("\t%d: %s %s(){}\n", f->line, f->type, f->name);
    }
}
