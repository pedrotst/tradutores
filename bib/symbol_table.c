#include <stdio.h>
#include "symbol_table.h"


/*
    ---------------------- # CT Building Functions # -------------------------
*/

void build_ct(Program *p){
    Class *c, *tmp, *super;

    // Construa a CT somente para programas validos
    if(p == NULL)
        return;

    // Primeiro adicione Object na ct
    c = (Class*) malloc(sizeof(Class));
    c->selfName = strdup("Object");
    c->line = 0;
    c->functions = NULL;
    c->fields = NULL;
    c->super= NULL;
    DEBUG_PRINT("Coloca %s na ct\n", c->selfName);
    HASH_ADD_KEYPTR(hh, ct, c->selfName, strlen(c->selfName), c);

    // Adicione cada classe na CT
    ClassDecl *cdecl = p->classes;
    while(cdecl != NULL){
        HASH_FIND_STR(ct, cdecl->selfName, tmp); // esta classe ja foi declarada?
        HASH_FIND_STR(ct, cdecl->superName, super);
        if(tmp != NULL){
            printf("Line %d: WARN Class %s already declared at line %d, this declaration will be disconsidered\n", cdecl->line, tmp->selfName, tmp->line);
        }
        else if(super == NULL){
            printf("Line %d: ERROR Class %s is not a defined class\n", cdecl->line, cdecl->superName);
        }
        else{
            c = (Class*) malloc(sizeof(Class));
            c->selfName = cdecl->selfName;
            c->super= super;
            c->line = cdecl->line;
            c->functions = NULL;
            c->fields = NULL;
            build_class_body(c, cdecl->cMembers);
            DEBUG_PRINT("Coloca %s na ct\n", c->selfName);
            HASH_ADD_KEYPTR(hh, ct, c->selfName, strlen(c->selfName), c);
        }
        cdecl = cdecl->next;
    }
    return;

}

void build_class_body(Class *c, ClassMember *cmem){
    while(cmem != NULL){
        if(cmem->utype == VAR_DECL)
            hash_insert_variable(cmem->member->varDecls, &(c->fields));
        else if(cmem->utype == FUN_DECL)
            hash_insert_function(cmem->member->funDecl, &(c->functions), c);

        cmem = cmem->next;
    }

}

/**
    Preciso passar Variable **v_table pois HASH_ADD pode modificar o valor de *v_table
*/
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


void hash_insert_function(FunctionDecl *funs, Function **f_table, Class *c){
    Function *f;
    HASH_FIND_STR(*f_table, funs->name, f);
    if(f == NULL){
        f = (Function*)malloc(sizeof(Function));
        f->name = funs->name;
        f->type = funs->type;
        f->line = funs->line;
        f->vars = NULL;
        f->this = c;
        f->stmts = funs->stmts;
        f->fargs = funs->fargs;
        HASH_ADD_KEYPTR(hh, *f_table, f->name, strlen(f->name), f);
    }
    else{
            printf("WARN %d: Function %s already declared at line %d at class %s, this declaration will be disconsidered\n", funs->line, funs->name, f->line, f->this->selfName);
    }
}


/*
    ---------------------- # Print Functions # -------------------------
*/

void print_ct(){
   Class *c = NULL;
   /*
   HASH_FIND_STR(ct, "a", c);
   printf("Achei %d:%s extd %s na ct\n", c->line, c->selfName, c->superName);
   */
   printf("------------------ # Class Table # ------------------\n");
   for(c=ct; c != NULL; c = (c->hh.next)){
       // Don't print if its object
       if(strcmp(c->selfName, "Object")) {
           printf("%d: %s extd %s{\n", c->line, c->selfName, c->super->selfName);
           print_vars(c->fields);
           print_functions(c->functions);
           printf("}\n");
       }
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
