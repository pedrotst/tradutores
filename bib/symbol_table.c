#include <stdio.h>
#include "symbol_table.h"


/*
    ---------------------- # CT Building Functions # -------------------------
*/

void add_Object_ct(){
    Class *c;
    c = (Class*) malloc(sizeof(Class));
    c->selfName = strdup("Object");
    c->line = 0;
    c->functions = NULL;
    c->fields = NULL;
    c->superName = NULL;
    c->super= NULL;
    DEBUG_PRINT("Coloca %s na ct\n", c->selfName);
    HASH_ADD_KEYPTR(hh, ct, c->selfName, strlen(c->selfName), c);
}

Class* resolve_type(char *type){
    Class *t;
    t = NULL;
    if(strcmp(type, "int") || strcmp(type, "bool")){
        HASH_FIND_STR(ct, type, t);
    }
    return t;
}

void build_ct(Program *p){
    Class *c, *tmp, *super;

    // Construa a CT somente para programas validos
    if(p == NULL)
        return;

    add_Object_ct();

    ClassDecl *cdecl = p->classes;
    while(cdecl != NULL){
        // esta classe ja foi declarada?
        HASH_FIND_STR(ct, cdecl->selfName, tmp);         
        HASH_FIND_STR(ct, cdecl->superName, super);
        if(tmp != NULL){
            printf("Line %d: WARN Class %s already declared at line %d, this declaration will be disconsidered\n", cdecl->line, tmp->selfName, tmp->line);
        } else{
            c = (Class*) malloc(sizeof(Class));
            c->selfName = cdecl->selfName;
            c->super = super;
            c->superName = cdecl->superName;
            c->line = cdecl->line;
            c->functions = NULL;
            c->fields = NULL;
            build_class_body(c, cdecl->cMembers);
            DEBUG_PRINT("Coloca %s na ct\n", c->selfName);
            HASH_ADD_KEYPTR(hh, ct, c->selfName, strlen(c->selfName), c);
        } 
        if(super == NULL)
            printf("Line %d: ERROR Class %s is not a defined class\n", cdecl->line, cdecl->superName);
        cdecl = cdecl->next;
    }
    return;

}

void build_class_body(Class *c, ClassMember *cmem){
    while(cmem != NULL){
        if(cmem->utype == VAR_DECL)
            hash_insert_varDecl(cmem->member->varDecls, &(c->fields));
        else if(cmem->utype == FUN_DECL)
            hash_insert_function(cmem->member->funDecl, &(c->functions), c);

        cmem = cmem->next;
    }

}

/**
    Preciso passar Variable **v_table pois HASH_ADD pode modificar o valor de *v_table
*/
void hash_insert_varDecl(VarDecl *vars, Variable **v_table){
    Variable *v, *tmp;
    IdList *ids = vars->idList;
    while(ids != NULL){
        HASH_FIND_STR(*v_table, ids->id, tmp); // esta variavel ja foi declarada?
        if(tmp == NULL){
            v = (Variable*)malloc(sizeof(Variable));
            v->name = ids->id;
            v->type = vars->type;
            v->tref = resolve_type(vars->type);
            v->line = vars->line;
            v->ch_begin = ids->ch_begin;
            v->ch_end = ids->ch_end;
            HASH_ADD_KEYPTR(hh, *v_table, v->name, strlen(v->name), v);
        }else{
            printf("WARN %d-[%d-%d]: Variable %s already declared at line %d, this declaration will be disconsidered\n", tmp->line, tmp->ch_begin, tmp->ch_end, tmp->name, vars->line);
        }
        ids = ids->next;
    }
    
}

void hash_insert_fargs(FormalArgs *fargs, Variable **v_table){
    Variable *v, *tmp;
    while(fargs != NULL){
        HASH_FIND_STR(*v_table, fargs->name, tmp); // esta variavel ja foi declarada?
        if(tmp == NULL){
            v = (Variable*)malloc(sizeof(Variable));
            v->name = fargs->name;
            v->type = fargs->type;
            v->tref = resolve_type(fargs->type);
            v->line = fargs->line;
            v->ch_begin = fargs->ch_begin;
            v->ch_end = fargs->ch_end;
            HASH_ADD_KEYPTR(hh, *v_table, v->name, strlen(v->name), v);
        }else{
            printf("WARN %d-[%d-%d]: Variable %s already declared at line %d, this declaration will be disconsidered\n", fargs->line, fargs->ch_begin, fargs->ch_end, fargs->name, tmp->line);
        }
        fargs = fargs->next;
    }
}

void hash_insert_function(FunctionDecl *funs, Function **f_table, Class *c){
    Function *f;
    HASH_FIND_STR(*f_table, funs->name, f);
    if(f == NULL){
        f = (Function*)malloc(sizeof(Function));
        f->name = funs->name;
        f->type = funs->type;
        f->tref = resolve_type(funs->type);
        f->line = funs->line;
        f->this = c;
        f->stmts = funs->stmts;
        f->fargs = funs->fargs;
        // insert the arguments to the function var table
        hash_insert_fargs(funs->fargs, &(f->vars));
        check_stmts(funs->stmts, f);

        HASH_ADD_KEYPTR(hh, *f_table, f->name, strlen(f->name), f);
    }
    else{
        printf("WARN %d: Function %s already declared at line %d at class %s, this declaration will be disconsidered\n", funs->line, funs->name, f->line, f->this->selfName);
    }
}

void check_stmts(StmtList *stmts, Function *f){
    while(stmts != NULL){
        if(stmts->stmt->utype == VAR_DECL)
            hash_insert_varDecl(stmts->stmt->stmt_u->varDecl, &(f->vars));
        else if(stmts->stmt->utype == IF_STMT){
            check_bool(stmts->stmt->stmt_u->ifStmt->cond, f);
        }
        stmts = stmts->next;
    }
}

void check_bool(Exp *e, Function *f){
    if(e->utype == VAR_EXP){
        Variable *decl_v = NULL;
        Var *v = e->exp_u->var;
        if(v->utype == ID_VAR){
            HASH_FIND_STR(f->vars, v->var_u->id, decl_v);
            // if variable is not within function scope look at class scope
            if(decl_v == NULL){
                HASH_FIND_STR(f->this->fields, v->var_u->id, decl_v);
            }
            // if variable isn't within class scope also then error
            if(decl_v == NULL){
                printf("ERROR %d[%d-%d]: used variable %s not declared\n", 0, 0, 0, v->var_u->id);
            } // is it declared as bool?
            else if(strcmp(decl_v->type, "bool"))
                printf("ERROR %d[%d-%d]: if condition must be boolean\n", 0,0,0);
        }
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
           printf("%d: %s extd %s{\n", c->line, c->selfName, c->superName);
           print_vars(c->fields, 1);
           print_functions(c->functions);
           printf("}\n");
       }
   }
}

void print_vars(Variable *vt, int tabs){
    Variable *v;
    for(v=vt; v != NULL; v = (v->hh.next)){
        print_tabs(tabs);
        printf("%d: %s %s[%d-%d];\n", v->line, v->type, v->name, v->ch_begin, v->ch_end);
    }
}

/*
void print_tabs(int tabs){
    for(int i; i < tabs; i++) printf("\t");
}*/


void print_functions(Function *ft){
    Function *f;
    for(f=ft; f != NULL; f = (f->hh.next)){
        printf("\t%d: %s %s(", f->line, f->type, f->name);
        print_fargs(f->fargs);
        printf("){\n");
        print_vars(f->vars, 2);
        printf("\t}\n");
    }
}
