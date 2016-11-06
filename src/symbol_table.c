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

Class* resolve_type(char *type, int line, int ch_begin, int ch_end){
    Class *t = NULL;
    if(strcmp(type, "int") && strcmp(type, "bool")){
        HASH_FIND_STR(ct, type, t);
        if(t == NULL){
            printf("ERROR %d:%d: class %s not declared\n", line, ch_begin, type);
            print_arq_line(line, ch_begin, ch_end);
        }

    }
    return t;
}

void add_raw_classes_ct(Program *p){
    Class *c, *tmp;
    ClassDecl *cdecl = p->classes;
    while(cdecl != NULL){
        HASH_FIND_STR(ct, cdecl->selfName, tmp);         
        if(tmp != NULL){
            int ch_len = strlen("class ") + 1;
            printf("WARN %d: Class %s already declared\n", cdecl->line, tmp->selfName);
            print_arq_line(cdecl->line, ch_len, ch_len + strlen(cdecl->selfName) - 1);
            printf("Note %d: Last declaration was here\n", tmp->line);
            print_arq_line(tmp->line, ch_len, ch_len + strlen(tmp->selfName) - 1);
        }else{
            c = (Class*) malloc(sizeof(Class));
            c->selfName = cdecl->selfName;
            c->super = NULL;
            c->superName = cdecl->superName;
            c->line = cdecl->line;
            c->functions = NULL;
            c->fields = NULL;
            DEBUG_PRINT("Coloca %s na ct\n", c->selfName);
            HASH_ADD_KEYPTR(hh, ct, c->selfName, strlen(c->selfName), c);
        } 
        cdecl = cdecl->next;
    }
}

void build_ct(Program *p){
    Class *c, *tmp, *super;

    // Construa a CT somente para programas validos
    if(p == NULL)
        return;

    add_Object_ct();
    add_raw_classes_ct(p);

    ClassDecl *cdecl = p->classes;
    while(cdecl != NULL){
        // esta classe ja foi declarada?
        HASH_FIND_STR(ct, cdecl->selfName, tmp);         
        HASH_FIND_STR(ct, cdecl->superName, super);
        if(super != NULL){
            tmp->super = super;
            build_class_body(tmp, cdecl->cMembers);
        } else {
            int ch_len = strlen("class  extends ") + strlen(cdecl->selfName) + 1;
            printf("Line %d: ERROR Class %s is not a defined class at\n", cdecl->line, cdecl->superName);
            print_arq_line(cdecl->line, ch_len, ch_len+strlen(cdecl->superName) - 1);
        }
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
    Class *c = resolve_type(vars->type, vars->line, vars->type_begin, vars->type_end);
    while(ids != NULL){
        HASH_FIND_STR(*v_table, ids->id, tmp); // esta variavel ja foi declarada?
        if(tmp == NULL){
            v = (Variable*)malloc(sizeof(Variable));
            v->name = ids->id;
            v->type = vars->type;
            v->tref = c;
            v->line = vars->line;
            v->ch_begin = ids->ch_begin;
            v->ch_end = ids->ch_end;
            HASH_ADD_KEYPTR(hh, *v_table, v->name, strlen(v->name), v);
        }else{
            printf("WARN %d:%d: Redeclaration of %s \n", vars->line, ids->ch_begin, tmp->name);
            print_arq_line(vars->line, ids->ch_begin, ids->ch_end);
            printf("Note %d:%d: Last declaration of %s was here\n", tmp->line, tmp->ch_begin, tmp->name);
            print_arq_line(tmp->line, tmp->ch_begin, tmp->ch_end);

        }
        ids = ids->next;
    }
    
}

void print_arq_line(int line, int ch_begin, int ch_end){
    int i;

    printf("%s", source[line - 1]);

    for(i=0; i < ch_begin - 1; i++){
        if(source[line -1][i] == '\r')
            printf("\r");
        else if(source[line -1][i] == '\t')
            printf("\t");
        else
            printf(" ");
    }
    for(; i < ch_end; i++)
        printf("^");

    printf("\n");


}

void hash_insert_fargs(FormalArgs *fargs, Variable **v_table){
    Variable *v, *tmp;
    while(fargs != NULL){
        HASH_FIND_STR(*v_table, fargs->name, tmp); // esta variavel ja foi declarada?
        if(tmp == NULL){
            v = (Variable*)malloc(sizeof(Variable));
            v->name = fargs->name;
            v->type = fargs->type;
            v->tref = resolve_type(fargs->type, fargs->line, fargs->type_begin, fargs->type_end);
            v->line = fargs->line;
            v->ch_begin = fargs->ch_begin;
            v->ch_end = fargs->ch_end;
            HASH_ADD_KEYPTR(hh, *v_table, v->name, strlen(v->name), v);
        }else{
            printf("WARN %d: Variable %s already declared\n", fargs->line, fargs->name);
            print_arq_line(fargs->line, fargs->ch_begin , fargs->ch_end);
            printf("Note %d:%d: Last declaration was here\n", tmp->line, tmp->ch_begin);
            print_arq_line(tmp->line, tmp->ch_begin, tmp->ch_end);
        }
        fargs = fargs->next;
    }
}

void hash_insert_function(FunctionDecl *funs, Function **f_table, Class *c){
    Function *f;
    HASH_FIND_STR(*f_table, funs->name, f); // funcao declarada?
    if(f == NULL){
        f = (Function*)malloc(sizeof(Function));
        f->name = funs->name;
        f->type = funs->type;
        f->tref = resolve_type(funs->type, funs->line, funs->type_begin, funs->type_end);
        f->line = funs->line;
        f->name_begin = funs->name_begin;
        f->name_end = funs->name_end;
        f->this = c;
        f->stmts = funs->stmts;
        f->fargs = funs->fargs;
        // insert the arguments to the function var table
        hash_insert_fargs(funs->fargs, &(f->vars));
        check_stmts(funs->stmts, f);

        HASH_ADD_KEYPTR(hh, *f_table, f->name, strlen(f->name), f);
    }
    else{
        int ch_len = strlen(funs->type) + 2;
        printf("WARN %d: Function %s already declared\n", funs->line, funs->name);
        print_arq_line(funs->line, funs->name_begin, funs->name_end);
        ch_len = strlen(f->type) + 2;
        printf("Note %d: Last declaration was here\n", f->line);
        print_arq_line(f->line, f->name_begin, f->name_end);
    }
}

void check_stmts(StmtList *stmts, Function *f){
    while(stmts != NULL){
        if(stmts->utype == VAR_DECL)
            hash_insert_varDecl(stmts->stmt_u->varDecl, &(f->vars));
        else if(stmts->utype == IF_STMT){
            check_bool(stmts->stmt_u->ifStmt->cond, f);
        }
        else if(stmts->utype == ASSGN_STMT){
            check_assignment(stmts->stmt_u->assgn, f);
        }
        stmts = stmts->next;
    }
}

void check_assignment(Assignment *assgn, Function *f){
    if(assgn->lhs->utype == OBJ_VAR){
        if(assgn->lhs->var_u->obj->utype == METH_OBJ){
            printf("Error %d: cannot assign a value to a method invocation in\n", assgn->line);
            print_arq_line(assgn->line, 0, 0);
        }
        else if(assgn->lhs->var_u->obj->utype == NEW_OBJ){
            printf("Error %d: cannot assign a value an object creation in\n", assgn->line);
            print_arq_line(assgn->line, 0, 0);
        }
    }
    else{
        char *lhs_type = var_type(assgn->lhs, f);
        char *rhs_type = exp_type(assgn->rhs, f);
        if(strcmp(lhs_type, rhs_type)){
            printf("Error %d: lhs of assignment of different type of rhs in\n", assgn->line);
            print_arq_line(assgn->line, 0, 0);
            printf("Note: lhs of assignment of type %s and rhs of type %s\n", lhs_type, rhs_type);
        }
    }
}

char* var_type(Var *v, Function *f){
    Class *c_decl;
    Variable *v_decl = NULL;
    char *obj_type;
    if(v->utype == ID_VAR){ 
        // 1) look on class scope
        HASH_FIND_STR(f->vars, v->var_u->id, v_decl);
        // 2) then look on field scope
        if(v_decl == NULL)
            v_decl = class_get_field(f->this, v->var_u->id);
        if(v_decl == NULL){
            printf("Error %d:%d: variable %s was not declared at\n", v->line, v->ch_begin, v->var_u->id);
            print_arq_line(v->line, v->ch_begin, v->ch_end);

        }
        else
            return v_decl->type;
    }
    else if(v->utype == OBJ_VAR){
        if(v->var_u->obj->utype == NEW_OBJ){ // has to check for the arguments type yet
            return v->var_u->obj->obj_u->newObj->cname;
        }
        else if(v->var_u->obj->utype == METH_OBJ){
            MethodInvoc *m_invk = v->var_u->obj->obj_u->meth;
            Function *f_decl;
            // 1) encontrar o tipo do objeto
            obj_type = var_type(m_invk->obj, f);
            // 2) encontrar a declaracao daquela classe 
            HASH_FIND_STR(ct, obj_type, c_decl);
            // 3) encontrar a funcao na declaracao daquela classe 
            HASH_FIND_STR(c_decl->functions, m_invk->mname, f_decl);
            // 4) Match the arg types with the fargs types
            // 5) retornar o tipo do retorno
            if(f_decl == NULL){
                printf("Error %d:%d: class %s does not have a method %s\n", v->line, v->ch_begin, c_decl->selfName, m_invk->mname);
                print_arq_line(v->line, v->ch_begin, v->ch_end);
            }
            else 
                return f_decl->type;

        }
        else if(v->var_u->obj->utype == FIELD_OBJ){
            // 1) encontrar o tipo do objeto
            // 2) encontrar a declaracao daquela classe
            // 3) retornar o tipo do field
        }
    }
    return "??";
}

char* exp_type(Exp *e, Function *f){
    if(e->utype == VAR_EXP)
        return var_type(e->exp_u->var, f);
    else if(e->utype == BINOP_EXP){
        BinOp *b_op = e->exp_u->binOp;
        char op = b_op->op;
        check_binOp(b_op, f);
        return op_type(op);
    }
    else if(e->utype == PAR_EXP)
        return exp_type(e->exp_u->parenthesis, f);
    else if(e->utype == PRIM_EXP){
        return e->exp_u->primary->type;
    }

    return "??";
}

char* op_type(char op){
    if(op == '&' || op == '(' || op == '!' || op == '|' 
        || op == '=' || op == ')' || op == '>' || op == '<')
        return "bool";
    else if(op == '+' || op == '-' || op == '*' || op == '/')
        return "int";
    return "??";
}
    

int check_binOp(BinOp *b, Function *f){
    char *lhs_type = exp_type(b->lhs, f);
    char *rhs_type = exp_type(b->rhs, f);
    char *optype = op_type(b->op);
    if(!strcmp(optype, lhs_type)){
        if(!strcmp(optype, rhs_type))
            return 1;
        else{
            printf("Error %d: rhs of operation is not %s\n", b->rhs->line, optype);
            print_arq_line(b->rhs->line, b->rhs->ch_begin, b->rhs->ch_end);
            return 0;
        }
    }
    else{
        printf("Error %d: lhs of operation is not %s\n", b->lhs->line, optype);
        print_arq_line(b->lhs->line, b->lhs->ch_begin, b->lhs->ch_end);
        if(strcmp(optype, rhs_type)){
            printf("Error %d: rhs of operation is not %s\n", b->rhs->line, optype);
            print_arq_line(b->rhs->line, b->rhs->ch_begin, b->rhs->ch_end);
        }
        return 0;
    }
    return 0;
}

void check_bool(Exp *e, Function *f){
    if(e->utype == VAR_EXP){
        Variable *decl_v = NULL;
        Var *v = e->exp_u->var;
        if(v->utype == ID_VAR){
            decl_v = class_get_field(f->this, v->var_u->id);
            if(decl_v == NULL){
                printf("ERROR %d:%d: used variable %s not declared\n", v->line, v->ch_begin, v->var_u->id);
                print_arq_line(v->line, v->ch_begin, v->ch_end);
            } // is it declared as bool?
            else if(strcmp(decl_v->type, "bool")){
                printf("ERROR %d:%d: if condition must be boolean, but it is actually %s\n", v->line, v->ch_begin, decl_v->type);
                print_arq_line(v->line, v->ch_begin, v->ch_end);
                printf("Note %d:%d: it was declared here\n", decl_v->line, decl_v->ch_begin);
                print_arq_line(decl_v->line, decl_v->ch_begin, decl_v->ch_end);
            }
        }
    }
}

Variable* class_get_field(Class* c, char* id){
    Variable *decl_v = NULL;

    HASH_FIND_STR(c->fields, id, decl_v);
    // try to find field on superClasses
    c = c->super; 
    while(decl_v == NULL && c != NULL){
        // object->super is NULL; i.e. end of recursion
        HASH_FIND_STR(c->fields, id, decl_v);
        c = c->super; 
    }

    return decl_v;
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
