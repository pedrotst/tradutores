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
    if(type == NULL)
        return NULL;

    if(strcmp(type, "int") && strcmp(type, "bool")){
        HASH_FIND_STR(ct, type, t);
        if(t == NULL){
            printf("ERROR %d:%d: class %s not declared at\n", line, ch_begin, type);
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
            printf("Note %d: Last declaration was here\n\n", tmp->line);
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
            Function *f;

            f = (Function*) malloc(sizeof(Function));
            f->name = strdup("super");
            f->type = NULL;
            f->tref = NULL;
            f->fargs = NULL;
            f->vars = NULL;
            f->stmts = NULL;
            f->line = f->type_begin = f->type_end = f->name_begin = f->name_end = 0;
            f->this = c;


            HASH_ADD_KEYPTR(hh, c->functions, f->name, strlen(f->name), f);
            HASH_ADD_KEYPTR(hh, ct, c->selfName, strlen(c->selfName), c);
        } 
        cdecl = cdecl->next;
    }
}

int build_ct(Program *p){
    Class *c, *tmp, *super;
    sem_errs = 0;
    // Construa a CT somente para programas validos
    if(p == NULL)
        return sem_errs;

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
            printf("Error %d: Class %s is not a defined class at\n", cdecl->line, cdecl->superName);
            print_arq_line(cdecl->line, ch_len, ch_len+strlen(cdecl->superName) - 1);
            sem_errs++;
        }
        cdecl = cdecl->next;
    }

    Function *main=(Function*)malloc(sizeof(Function));
    HASH_FIND_STR(ct, "Object", tmp);         
    main->name = strdup("main");
    main->type = NULL;
    main->tref = NULL;
    main->line = main->name_begin = main->name_end = 0;
    main->vars = NULL;
    main->stmts = p->stmts;
    main->fargs = NULL;
    main->this = tmp;
    check_stmts(p->stmts, main);
    HASH_ADD_KEYPTR(hh, tmp->functions, main->name, strlen(main->name), main);

    return sem_errs;

}

void add_raw_funs_class(Class *c, ClassMember *cmem){
    Function *f;
    FunctionDecl *funs;

    while(cmem != NULL){
        if(cmem->utype == FUN_DECL){
            funs = cmem->member->funDecl;
            HASH_FIND_STR(c->functions, funs->name, f); // funcao declarada?
            if(f == NULL){
                DEBUG_PRINT("add fun %s to class %s\n", funs->name, c->selfName);
                f = (Function*) malloc(sizeof(Function));   
                f->vars = NULL;

                f->name = funs->name;
                f->type = funs->type;
                f->tref = NULL;
                f->line = funs->line;
                f->name_begin = funs->name_begin;
                f->name_end = funs->name_end;
                f->type_begin = funs->type_begin;
                f->type_end = funs->type_end;
                f->this = c;
                f->stmts = funs->stmts;
                f->fargs = funs->fargs;
                f->vars = NULL;
                HASH_ADD_KEYPTR(hh, c->functions, f->name, strlen(f->name), f);
            }
            else{
                printf("WARN %d: Function %s already declared in class %s\n", funs->line, funs->name, c->selfName);
                print_arq_line(funs->line, funs->name_begin, funs->name_end);
                printf("Note %d: Last declaration was here\n\n", f->line);
                print_arq_line(f->line, f->name_begin, f->name_end);
            }
        
        }
        cmem = cmem->next;
    }
}

void build_class_body(Class *c, ClassMember *cmem){
    add_raw_funs_class(c, cmem);
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
            printf("Note %d:%d: Last declaration of %s was here\n\n", tmp->line, tmp->ch_begin, tmp->name);
            print_arq_line(tmp->line, tmp->ch_begin, tmp->ch_end);

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
            v->tref = resolve_type(fargs->type, fargs->line, fargs->type_begin, fargs->type_end);
            v->line = fargs->line;
            v->ch_begin = fargs->ch_begin;
            v->ch_end = fargs->ch_end;
            HASH_ADD_KEYPTR(hh, *v_table, v->name, strlen(v->name), v);
        }else{
            printf("WARN %d: Variable %s already declared\n", fargs->line, fargs->name);
            print_arq_line(fargs->line, fargs->ch_begin , fargs->ch_end);
            printf("Note %d:%d: Last declaration was here\n\n", tmp->line, tmp->ch_begin);
            print_arq_line(tmp->line, tmp->ch_begin, tmp->ch_end);
        }
        fargs = fargs->next;
    }
}

void hash_insert_function(FunctionDecl *funs, Function **f_table, Class *c){
    Function *f;
    Variable *v;

    // if function has constr name, sem_errs return
    if(funs->type != NULL && !strcmp(funs->name, c->selfName)){
        printf("Error %d: only constructor may have the same name as the class\n", funs->line);
        print_arq_line(f->line, 0, 0);
        sem_errs++;
        return;
    }

    HASH_FIND_STR(*f_table, funs->name, f); // funcao declarada?
     
    if(f != NULL){
        f->tref = resolve_type(funs->type, funs->line, funs->type_begin, funs->type_end);

    // insert this
    Variable *v = (Variable*)malloc(sizeof(Variable));
    v->name = strdup("this");
    v->type = c->selfName;
    v->tref = c;
    v->line = 0;
    v->ch_begin = 0;
    v->ch_end = 0;
    HASH_ADD_KEYPTR(hh, f->vars, v->name, 4, v);


        // insert the arguments to the function var table
        hash_insert_fargs(funs->fargs, &(f->vars));
        check_stmts(funs->stmts, f);
    }
    
    
}

void check_stmts(StmtList *stmts, Function *f){
    while(stmts != NULL){
        if(stmts->utype == VAR_DECL)
            hash_insert_varDecl(stmts->stmt_u->varDecl, &(f->vars));
        else if(stmts->utype == IF_STMT){
            check_bool(stmts->stmt_u->ifStmt->cond, f);
            check_stmts(stmts->stmt_u->ifStmt->then, f);
            check_stmts(stmts->stmt_u->ifStmt->els, f);
        }else if(stmts->utype == WHILE_STMT){
            check_bool(stmts->stmt_u->whileStmt->cond, f);
            check_stmts(stmts->stmt_u->whileStmt->loop, f);
        } else if(stmts->utype == ASSGN_STMT){
            check_assignment(stmts->stmt_u->assgn, f);
        } else if(stmts->utype == VAR_STMT){
            var_type(stmts->stmt_u->varStmt, f);
        } else if(stmts->utype == RET_STMT){
            check_return(stmts->stmt_u->returnExp, f);
            if(stmts->next != NULL){
                printf("WARN %d: unreachable code after return\n", stmts->line);
            }
        }
        stmts = stmts->next;
    }
}

void check_return(Exp *e, Function *f){
    char *e_type = exp_type(e, f);
    if(f->type == NULL){
        printf("Error %d: main and constructors cannot have a return at\n", e->line);
        print_arq_line(e->line, e->ch_begin, e->ch_begin);
        sem_errs++;

    }
    else if(strcmp(e_type, f->type)){
        printf("Error %d: function %s must return some type %s, but was %s at\n", e->line, f->name, f->type, e_type);
        print_arq_line(e->line, e->ch_begin, e->ch_begin);
        printf("Note %d: function signature is here\n", f->line);
        print_arq_line(f->line, f->type_begin, f->type_begin);
        sem_errs++;
    }
}

void check_assignment(Assignment *assgn, Function *f){
    if(assgn->lhs->utype == OBJ_VAR
        && assgn->lhs->var_u->obj->utype != FIELD_OBJ){ // if its field_obj, then treat as a variable
        if(assgn->lhs->var_u->obj->utype == METH_OBJ){
            printf("Error %d: cannot assign a value to a method invocation in\n", assgn->line);
            print_arq_line(assgn->line, 0, 0);
        sem_errs++;
        }
        else if(assgn->lhs->var_u->obj->utype == NEW_OBJ){
            printf("Error %d: cannot assign a value an object creation in\n", assgn->line);
            print_arq_line(assgn->line, 0, 0);
        sem_errs++;
        }
    }
    else{
        char *lhs_type = var_type(assgn->lhs, f);
        char *rhs_type = exp_type(assgn->rhs, f);
        if(strcmp(lhs_type, rhs_type)){
            printf("Error %d: lhs of assignment has different type then rhs in\n", assgn->line);
            print_arq_line(assgn->line, 0, 0);
            printf("Note: lhs of assignment has type %s and rhs has type %s\n\n", lhs_type, rhs_type);
        sem_errs++;
        }
    }
}

char* var_type(Var *v, Function *f){
    Class *c_decl;
    Function *f_decl;
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
            sem_errs++;
            return "??";

        }
        else
            return v_decl->type;
    }
    else if(v->utype == OBJ_VAR){
        if(v->var_u->obj->utype == NEW_OBJ){ // has to check for the arguments type yet
            New *obj = v->var_u->obj->obj_u->newObj;
            HASH_FIND_STR(ct, obj->cname, c_decl);
            if(c_decl != NULL){
                HASH_FIND_STR(c_decl->functions, obj->cname, f_decl);
                if(f_decl != NULL){
                    function_check_argTypes(f_decl, obj->args, f, v->line);
                }
            } else {
                if(obj->args != NULL){
                    printf("Error %d: class %s constructor expected 0 arguments at\n", v->line, obj->cname);
                    print_arq_line(v->line, 0, 0);
                    sem_errs++;
                }
            }
            return obj->cname;
        }
        else if(v->var_u->obj->utype == METH_OBJ){
            MethodInvoc *m_invk = v->var_u->obj->obj_u->meth;
            Function *f_decl;
            // 1) encontrar o tipo do objeto
            if(m_invk->obj == NULL)
                obj_type = f->this->selfName;
            else
                obj_type = var_type(m_invk->obj, f);

            if(!strcmp(obj_type, "int") || !strcmp(obj_type, "int")){
                printf("Error %d: Primitive types does not have methods at\n", v->line);
                print_arq_line(v->line, v->ch_begin, v->ch_end);
                sem_errs++;
                return "??";
            }
            // 2) encontrar a declaracao daquela classe 
            HASH_FIND_STR(ct, obj_type, c_decl);
            // 3) encontrar o metodo na declaracao daquela classe 
            f_decl = class_get_function(c_decl, m_invk->mname);
            if(f_decl == NULL){
                printf("Error %d:%d: class %s does not have a method %s\n", v->line, v->ch_begin, c_decl->selfName, m_invk->mname);
                print_arq_line(v->line, m_invk->name_begin, m_invk->name_end);
                sem_errs++;
                return "??";
            }
            // 4) Match the arg types with the fargs types
            function_check_argTypes(f_decl, m_invk->args, f,v->line);
            // 5) retornar o tipo do retorno
            return f_decl->type;

        }
        else if(v->var_u->obj->utype == FIELD_OBJ){
            FieldAccess *f_acc = v->var_u->obj->obj_u->field;
            // 1) encontrar o tipo do objeto
            obj_type = var_type(f_acc->obj, f);
            HASH_FIND_STR(ct, obj_type, c_decl);
            v_decl = class_get_field(c_decl, f_acc->fname);
            if(v_decl == NULL){
                printf("Error %d:%d: class %s does not have a field %s\n", v->line, v->ch_begin, f->this->selfName, f_acc->fname);
                print_arq_line(v->line, v->ch_begin, v->ch_end);
                sem_errs++;
                return "??";
            }
            else 
            // 3) retornar o tipo do field
                return v_decl->type;


        }
    }
    return "??";
}

void function_check_argTypes(Function *f, ArgList *args, Function *scope, int line){
    FormalArgs *fargs = f->fargs;
    Exp *arg;
    char *argType;
    int arg_num = 0;

    while(fargs != NULL && args != NULL){
        arg = args->arg;
        argType = exp_type(args->arg, scope);
        if(!isSubClass(argType, fargs->type)){
            printf("Error %d:%d: method %s expected argument of type %s but received has type %s at \n", arg->line, arg->ch_begin, f->name, fargs->type, argType);
            print_arq_line(arg->line, arg->ch_begin, arg->ch_end);
            sem_errs++;
        }
        args = args->next;
        fargs = fargs->next;
        arg_num++;
    }

    if(fargs != NULL){
        int farg_num = arg_num;
        while(fargs != NULL){
            fargs = fargs->next;
            farg_num++;
        }
        printf("Error %d: method %s expected %d arguments but %d was given\n", line, f->name, farg_num, arg_num);
        print_arq_line(line, 0, 0);
        sem_errs++;
    } else if (args != NULL){
        printf("Error %d: method %s expected %d arguments ", line, f->name, arg_num);
        while(args != NULL){
            args = args->next;
            arg_num++;
        }
        printf("but %d was given\n", arg_num);
        print_arq_line(line, 0, 0);
        sem_errs++;
    }

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

int isSubClass(char *lhs, char *rhs){
    /*if(!strcmp(lhs, "int") || !strcmp(lhs, "bool"))
        return 0;
    if(!strcmp(rhs, "int") || !strcmp(rhs, "bool"))
        return 0;
    */
    Class *l, *r;

    HASH_FIND_STR(ct, lhs, l);
    HASH_FIND_STR(ct, rhs, r);
    if(l == NULL || r == NULL)
        return 0;

    while(l != NULL){
        if(!strcmp(l->selfName, r->selfName))
            return 1;
        l = l->super;
    }
    return 0;
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
            sem_errs++;
            return 0;
        }
    }
    else{
        printf("Error %d: lhs of operation is not %s\n", b->lhs->line, optype);
        print_arq_line(b->lhs->line, b->lhs->ch_begin, b->lhs->ch_end);
        sem_errs++;
        if(strcmp(optype, rhs_type)){
            printf("Error %d: rhs of operation is not %s\n", b->rhs->line, optype);
            print_arq_line(b->rhs->line, b->rhs->ch_begin, b->rhs->ch_end);
        }
        return 0;
    }
    return 0;
}

void check_bool(Exp *e, Function *f){
    char *e_type = exp_type(e,f);
    if(strcmp(e_type, "bool")){
        printf("Error %d:%d: expected a bool, but %s was given at\n", e->line, e->ch_begin, e_type);
        print_arq_line(e->line, e->ch_begin, e->ch_end);
        sem_errs++;
    }
}

Function* class_get_function(Class* c, char* id){
    if(c==NULL)
        return NULL;

    Function *f_decl = NULL;

    HASH_FIND_STR(c->functions, id, f_decl);
    // try to find field on superClasses
    c = c->super; 
    while(f_decl == NULL && c != NULL){
        // object->super is NULL; i.e. end of recursion
        HASH_FIND_STR(c->functions, id, f_decl);
        c = c->super; 
    }

    return f_decl;
}

Variable* class_get_field(Class* c, char* id){
    if(c==NULL)
        return NULL;

    Variable *v_decl = NULL;
    HASH_FIND_STR(c->fields, id, v_decl);
    // try to find field on superClasses
    c = c->super; 
    while(v_decl== NULL && c != NULL){
        // object->super is NULL; i.e. end of recursion
        HASH_FIND_STR(c->fields, id, v_decl);
        c = c->super; 
    }

    return v_decl;
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
        } else{
            Function *main = NULL;
            printf("Main:{\n");
            HASH_FIND_STR(c->functions, "main", main);
            print_vars(main->vars, 1);
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
        printf("\t%d: ", f->line);
        if(f->type != NULL)
            printf("%s ", f->type);
        printf("%s(", f->name);

        print_fargs(f->fargs);
        printf("){\n");
        print_vars(f->vars, 2);
        printf("\t}\n");
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

