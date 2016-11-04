#include "ast.h"

/**
     --------------------- # Tree Allocation # ---------------------
*/

Program* program_node(ClassDecl *classes, StmtList *stmts){
    Program *p = (Program*) malloc(sizeof(Program));
    if(p == NULL){
        err(1, "Could not allocate memory for program");
        exit(1);
    }
    p->classes = classes;
    p->stmts = stmts;


    return p;
}

ClassDecl* classDecl_node(char *selfName, char *superName,
        ClassMember *cMembers, ClassDecl *head,
        int line){

    ClassDecl *c = (ClassDecl*) malloc(sizeof(ClassDecl));
    if(c == NULL){
        err(1, "Could not allocate memory for Class");
        exit(1);
    }
    c->selfName = selfName;
    c->superName = superName;
    c->cMembers = cMembers;
    c->line = line;
    c->next= NULL;

    if(head == NULL)
        return c;

    ClassDecl *current = head;
    while(current->next != NULL){
        current = current->next;
    }

    current->next = c;



    return head;
}

ClassMember* classMembers_node(ClassMember *member, ClassMember *head){
    if(head == NULL)
        return member;

    ClassMember *current = head;
    while(current->next != NULL){
        current = current->next;
    }

    current->next = member;

    return head;
}

ClassMember* classMember_node(tag utype, VarDecl *varDecls,
    FunctionDecl *fundecl, ConstrDecl *constrDecl){

    ClassMember *c = (ClassMember*) malloc(sizeof(ClassMember));
    if(c == NULL){
        err(1, "Could not allocate memory for ClassMember");
        exit(1);
    }
    ClassMember_u *cmem = (ClassMember_u*) malloc(sizeof(ClassMember_u));
    switch(utype){
        case VAR_DECL:
            cmem->varDecls = varDecls;
        break;
        case FUN_DECL:
            cmem->funDecl = fundecl;
        break;
        case CONSTR_DECL:
            cmem->constrDecl = constrDecl;
        break;
        default:
            c->member = cmem;
            c->utype = utype;
        break;
    }
    c->utype = utype;
    c->member = cmem;

    return c;
}

VarDecl* varDecl_node(char *type, IdList *ids,
    int line, int type_begin, int type_end){
    VarDecl* v = (VarDecl*)malloc(sizeof(VarDecl));
    v->line = line;
    v->type = type;
    v->idList = ids;

    return v;

}

StmtList* stmtList_node(Stmt *stmt, StmtList *head, int line){
    StmtList* stmt_list = (StmtList*)malloc(sizeof(StmtList));
    stmt_list->stmt = stmt;
    stmt_list->line = line;
    stmt_list->next= NULL;

    if(head == NULL)
        return stmt_list;

    StmtList *current = head;
    while(current->next != NULL){
        current = current->next;
    }

    current->next = stmt_list;

    return head;

}


Stmt* stmt_node(tag utype, VarDecl *varDecl, IfStmt *ifStmt,
        WhileStmt *whileStmt, Exp *returnExp, Assignment *assgn){
    Stmt *stmt = (Stmt*)malloc(sizeof(Stmt));
    Stmt_u *stmt_u = (Stmt_u*)malloc(sizeof(Stmt_u));

    switch(utype){
        case VAR_DECL:
            stmt_u->varDecl = varDecl;
            break;
        case IF_STMT:
            stmt_u->ifStmt = ifStmt;
            break;
        case RET_STMT:
            stmt_u->returnExp = returnExp;
            break;
        case ASSGN_STMT:
            stmt_u->assgn = assgn;
            break;
        case WHILE_STMT:
            stmt_u->whileStmt = whileStmt;
            break;
        default:
        break;
    }
    stmt->utype = utype;
    stmt->stmt_u = stmt_u;

    return stmt;
}

ArgList* argList_node(Exp *arg, ArgList *head){
    ArgList *arg_list = (ArgList*)malloc(sizeof(ArgList));
    arg_list->arg = arg;
    arg_list->next = NULL;

    if(head == NULL)
        return arg_list;

    ArgList *current = head;
    while(current->next != NULL)
        current = current->next;
    current->next = arg_list;
    return head;
}

IdList* idList_node(char *id, int ch_begin, int ch_end, IdList *head){
    IdList* id_list = (IdList*)malloc(sizeof(IdList));
    id_list->id = id;
    id_list->ch_begin = ch_begin;
    id_list->ch_end = ch_end;
    id_list->next= NULL;

    if(head == NULL)
        return id_list;

    IdList *current = head;
    while(current->next != NULL){
        current = current->next;
    }

    current->next = id_list;

    return head;




}

WhileStmt* while_node(Exp *cond, StmtList *loop){
    WhileStmt *w = (WhileStmt*)malloc(sizeof(WhileStmt*));
    w->cond = cond;
    w->loop = loop;

    return w;
}

IfStmt* if_node(Exp *cond, StmtList *then, StmtList *els){
    IfStmt *i = (IfStmt*)malloc(sizeof(IfStmt*));
    i->cond = cond;
    i->then = then;
    i->els = els;

    return i;
}

FormalArgs* formalArgs_node(char *type, char *name, FormalArgs *head, int line, int ch_begin, int ch_end, int type_begin, int type_end){
    FormalArgs* fargs = (FormalArgs*)malloc(sizeof(FormalArgs));
    fargs->type = type;
    fargs->name = name;
    fargs->next = NULL;
    fargs->line = line;
    fargs->ch_begin = ch_begin;
    fargs->ch_end = ch_end;
    fargs->type_begin = type_begin;
    fargs->type_end = type_end;

    if(head == NULL)
        return fargs;

    FormalArgs* current = head;
    while(current->next != NULL)
        current = current->next;
    current->next = fargs;

    return head;
}

ConstrDecl* constrDecl_node(char *name,
    FormalArgs *fargs, StmtList *stmtList){

    ConstrDecl* c_decl = (ConstrDecl*)malloc(sizeof(ConstrDecl));

    c_decl->name = name;
    c_decl->fargs = fargs;
    c_decl->stmts = stmtList;

    return c_decl;
}

FunctionDecl* functionDecl_node(char *type, char *name,
    FormalArgs *fargs, StmtList *stmtList, int line,
    int name_begin, int name_end, int type_begin, int type_end){

    FunctionDecl* f_decl = (FunctionDecl*)malloc(sizeof(FunctionDecl));

    f_decl->name = name;
    f_decl->type = type;
    f_decl->line = line;
    f_decl->name_begin = name_begin;
    f_decl->name_end = name_end;
    
    f_decl->fargs = fargs;
    f_decl->stmts = stmtList;

    return f_decl;
}

Assignment* assignment_node(Var *lhs, Exp *rhs){
    Assignment *a = (Assignment*)malloc(sizeof(Assignment));
    a->lhs = lhs;
    a->rhs = rhs;

    return a;
}


Object* object_node(tag utype, FieldAccess *field,
    MethodInvoc *meth, New *newObj){
    Object *o = (Object*)malloc(sizeof(Object));
    Object_u *obj = (Object_u*)malloc(sizeof(Var_u));
    o->utype = utype;
    if(utype == FIELD_OBJ)
        obj->field = field;
    else if(utype == METH_OBJ)
        obj->meth = meth;
    else if(utype == NEW_OBJ)
        obj->newObj = newObj;

    o->obj_u = obj;
    return o;
}

Var* var_node(tag utype, char *id, Object *obj, int line, int ch_begin, int ch_end){
    Var *v = (Var*)malloc(sizeof(Var));
    Var_u *var = (Var_u*)malloc(sizeof(Var_u));
    v->utype = utype;
    v->line = line;
    v->ch_begin = ch_begin;
    v->ch_end = ch_end;
    if(utype == ID_VAR)
        var->id = id;
    else if(utype == OBJ_VAR)
        var->obj = obj;

    v->var_u = var;
    return v;

}

MethodInvoc* methodInvoc_node(Var *obj, char *mname, ArgList *args){
    MethodInvoc* m = (MethodInvoc*)malloc(sizeof(MethodInvoc));
    m->obj = obj;
    m->mname = mname;
    m->args = args;
    return m;
}

FieldAccess* fieldAccess_node(Var *obj, char *fname){
    FieldAccess* f = (FieldAccess*)malloc(sizeof(FieldAccess));
    f->obj = obj;
    f->fname = fname;
    return f;
}

New* new_node(char *cname, ArgList *args){
    New* n = (New*)malloc(sizeof(New));
    n->cname = cname;
    n->args = args;
    return n;
}

Exp* exp_node(tag utype, Var *var, BinOp *binOp,
        Exp *parenthesis, Primary *primary,
        int line, int ch_begin, int ch_end){
    Exp *e = (Exp*)malloc(sizeof(Exp));
    Exp_u *exp = (Exp_u*)malloc(sizeof(Exp_u));
    e->utype = utype;
    e->line = line;
    e->ch_begin = ch_begin;
    e->ch_end = ch_end;
    if(utype == VAR_EXP)
        exp->var = var;
    else if(utype == BINOP_EXP)
        exp->binOp = binOp;
    else if(utype == PAR_EXP)
        exp->parenthesis = parenthesis;
    else if(utype == PRIM_EXP)
        exp->primary = primary;
    e->exp_u = exp;
    return e;
}

BinOp* binOp_node(char op, Exp *lhs, Exp *rhs){
    BinOp *binOp = (BinOp*)malloc(sizeof(BinOp));
    binOp->op = op;
    binOp->lhs = lhs;
    binOp->rhs = rhs;

    return binOp;
}

Primary* primary_node(char *type, int val){
    Primary *primary = (Primary*)malloc(sizeof(Primary));
    primary->type = type;
    primary->val = val;
    return primary;
}

/**
     --------------------- # Tree Destruction # ---------------------
*/
void destruct_program(Program *p){
    if(p != NULL){
        destruct_classDecl(p->classes);
        destruct_stmtList(p->stmts);
        free(p);
    }
}

void destruct_classDecl(ClassDecl *cDecl){
    if(cDecl != NULL){
        free(cDecl->selfName);
        free(cDecl->superName);
        destruct_classMembers(cDecl->cMembers);
        destruct_classDecl(cDecl->next);
        free(cDecl);
    }
}

void destruct_stmtList(StmtList *sl){
    if(sl != NULL){
        destruct_stmt(sl->stmt);
        destruct_stmtList(sl->next);
        free(sl);
    }

}
void destruct_stmt(Stmt *stmt){
    if(stmt != NULL){
        if(stmt->utype == IF_STMT)
            destruct_ifStmt(stmt->stmt_u->ifStmt);
        else if(stmt->utype == RET_STMT)
            destruct_exp(stmt->stmt_u->returnExp);
        else if(stmt->utype == VAR_DECL)
            destruct_varDecl(stmt->stmt_u->varDecl);
        else if(stmt->utype == WHILE_STMT)
            destruct_whileStmt(stmt->stmt_u->whileStmt);
        else if(stmt->utype == ASSGN_STMT)
            destruct_assignment(stmt->stmt_u->assgn);

        free(stmt->stmt_u);
        free(stmt);
    }
}

void destruct_classMembers(ClassMember *cMem){
    if(cMem != NULL){
        if(cMem->utype == FUN_DECL)
            destruct_functionDecl(cMem->member->funDecl);
        else if(cMem->utype == VAR_DECL)
            destruct_varDecl(cMem->member->varDecls);
        else if(cMem->utype == CONSTR_DECL)
            destruct_constrDecl(cMem->member->constrDecl);
        free(cMem->member);
        destruct_classMembers(cMem->next);
        free(cMem);
    }
}

void destruct_functionDecl(FunctionDecl *funs){
    if(funs != NULL){
        free(funs->name);
        free(funs->type);
        destruct_formalArgs(funs->fargs);
        destruct_stmtList(funs->stmts);
        free(funs);
    }
}

void destruct_constrDecl(ConstrDecl *constrs){
    if(constrs != NULL){
        free(constrs->name);
        destruct_formalArgs(constrs->fargs);
        destruct_stmtList(constrs->stmts);
        free(constrs);
    }
}

void destruct_idList(IdList *ids){
    if(ids != NULL){
        free(ids->id);
        destruct_idList(ids->next);
        free(ids);
    }
}

void destruct_formalArgs(FormalArgs *fargs){
    if(fargs != NULL){
        free(fargs->type);
        free(fargs->name);
        destruct_formalArgs(fargs->next);
        free(fargs);
    }
}

void destruct_ifStmt(IfStmt *ifStmt){
    if(ifStmt != NULL){
        destruct_exp(ifStmt->cond);
        destruct_stmtList(ifStmt->then);
        destruct_stmtList(ifStmt->els);
        free(ifStmt);
    }
}

void destruct_whileStmt(WhileStmt *whileStmt){
    if(whileStmt != NULL){
        destruct_exp(whileStmt->cond);
        destruct_stmtList(whileStmt->loop);
        free(whileStmt);
    }
}

void destruct_exp(Exp *e){
    if(e != NULL){
        if(e->utype == VAR_EXP)
            destruct_var(e->exp_u->var);
        else if(e->utype == BINOP_EXP)
            destruct_binOp(e->exp_u->binOp);
        else if(e->utype == PAR_EXP)
            destruct_exp(e->exp_u->parenthesis);
        else if(e->utype == PRIM_EXP)
            destruct_primary(e->exp_u->primary);
        free(e->exp_u);
        free(e);
    }
}

void destruct_varDecl(VarDecl *vDecl){
    if(vDecl != NULL){
        free(vDecl->type);
        destruct_idList(vDecl->idList);
        free(vDecl);
    }
}

void destruct_assignment(Assignment *assgn){
    if(assgn != NULL){
       destruct_var(assgn->lhs);
       destruct_exp(assgn->rhs);
       free(assgn);
    }
}

void destruct_var(Var *v){
    if(v != NULL){
        if(v->utype == ID_VAR){
            free(v->var_u->id);
        }
        else if(v->utype == OBJ_VAR)
            destruct_object(v->var_u->obj);
        free(v->var_u);
        free(v);
    }
}

void destruct_object(Object *obj){
    if(obj != NULL){
        if(obj->utype == FIELD_OBJ)
            destruct_fieldAccess(obj->obj_u->field);
        else if(obj->utype == METH_OBJ)
            destruct_methodInvoc(obj->obj_u->meth);
        else if(obj->utype == NEW_OBJ)
            destruct_new(obj->obj_u->newObj);

        free(obj->obj_u);
        free(obj);
    }
}

void destruct_fieldAccess(FieldAccess *fAccess){
    if(fAccess != NULL){
        destruct_var(fAccess->obj);
        free(fAccess->fname);
        free(fAccess);
    }
}

void destruct_methodInvoc(MethodInvoc *mInvok){
    if(mInvok != NULL){
        destruct_var(mInvok->obj);
        free(mInvok->mname);
        destruct_argList(mInvok->args);
        free(mInvok);
    }
}

void destruct_new(New *new){
    if(new != NULL){
        free(new->cname);
        destruct_argList(new->args);
        free(new);
    }
}

void destruct_binOp(BinOp *op){
    if(op != NULL){
        destruct_exp(op->lhs);
        destruct_exp(op->rhs);
        free(op);
    }
}

void destruct_primary(Primary *prim){
    if(prim != NULL){
        free(prim);
    }
}

void destruct_argList(ArgList *args){
    if(args != NULL){
        destruct_exp(args->arg);
        destruct_argList(args->next);
        free(args);
    }
}


/**
     --------------------- # Tree Print # ---------------------
*/

void print_program(Program* p){
    print_class(p->classes);
    print_stmt(p->stmts, 0);
    printf("\n");
}

void print_class(ClassDecl *c){
    ClassDecl *cdecl = c;
    while(cdecl != NULL){
        printf("class %s extends %s{\n", cdecl->selfName, cdecl->superName);
        print_classMembers(cdecl->cMembers);
        printf("}\n");
        cdecl = cdecl->next;
    }

}

void print_classMembers(ClassMember *cmember){
    while(cmember != NULL){
        switch(cmember->utype){
            case VAR_DECL:
                print_varDecl(cmember->member->varDecls, 1);
                printf(";\n");
                break;
            case FUN_DECL:
                print_funDecl(cmember->member->funDecl);
                break;
            case CONSTR_DECL:
                print_constrDecl(cmember->member->constrDecl);
                break;
            default:
                break;
        }
        cmember = cmember->next;
    }
}

void print_varDecl(VarDecl *varDecls, int tabs){
    print_tabs(tabs);
    printf("%s ", varDecls->type);
    print_idList(varDecls->idList);
}

void print_constrDecl(ConstrDecl *constrDecl){
    if(constrDecl != NULL){
        printf("\t");
        printf("%s (", constrDecl->name);
        print_fargs(constrDecl->fargs);
        printf(") {\n");
        print_stmt(constrDecl->stmts, 2);
        printf("\t}\n");
    }
}

void print_funDecl(FunctionDecl *funDecl){
    if(funDecl != NULL){
        printf("\t");
        printf("%s ", funDecl->type);
        printf("%s (", funDecl->name);
        print_fargs(funDecl->fargs);
        printf(") {\n");
        print_stmt(funDecl->stmts, 2);
        printf("\t}\n");
    }
}

void print_fargs(FormalArgs *fargs){
    if(fargs != NULL){
        printf("%s %s", fargs->type, fargs->name);
        fargs = fargs->next;
    }

    while(fargs != NULL){
        printf(", %s %s", fargs->type, fargs->name);
        fargs = fargs->next;
    }
}

void print_idList(IdList *ids){
    // Don't forget to check if idList was created with a first element
    printf("%s", ids->id);
    ids = ids->next;
    while(ids!=NULL){
        printf(", %s", ids->id);
        ids = ids->next;
    }
}

void print_stmt(StmtList *stmts, int tabs){
    Stmt *stmt;
    while(stmts != NULL){
        stmt = stmts->stmt;
        switch(stmt->utype){
            case VAR_DECL:
                print_varDecl(stmt->stmt_u->varDecl, tabs);
                printf(";\n");
                break;
            case RET_STMT:
                print_return(stmt->stmt_u->returnExp, tabs);
                printf(";\n");
                break;
            case ASSGN_STMT:
                print_assignment(stmt->stmt_u->assgn, tabs);
                printf(";\n");
                break;
            case WHILE_STMT:
                print_while(stmt->stmt_u->whileStmt, tabs);
                break;
            case IF_STMT:
                print_if(stmt->stmt_u->ifStmt, tabs);
                break;
            default:
            break;
        }
        stmts = stmts->next;
    }

}

void print_if(IfStmt *i, int tabs){
    print_tabs(tabs);
    printf("if ");
    print_exp(i->cond);
    printf("{\n");
    print_stmt(i->then, tabs+1);
    printf("}\n");
    if(i->els != NULL){
        printf("else{\n");
        print_tabs(tabs);
        print_stmt(i->els, tabs+1);
        printf("}\n");
    }
}
void print_while(WhileStmt *w, int tabs){
    print_tabs(tabs);
    printf("while ");
    print_exp(w->cond);
    printf("{\n");
    print_stmt(w->loop, tabs+1);
    printf("}\n");
}

void print_return(Exp *e, int tabs){
    print_tabs(tabs);
    printf("return ");
    print_exp(e);
}

void print_exp(Exp *e){
    if(e != NULL){
        if(e->utype == VAR_EXP)
            print_var(e->exp_u->var);
        else if(e->utype == BINOP_EXP)
            print_binOp(e->exp_u->binOp);
        else if(e->utype == PAR_EXP){
            printf("(");
            print_exp(e->exp_u->parenthesis);
            printf(")");
        }
        else if(e->utype == PRIM_EXP)
            print_prim(e->exp_u->primary);
    }
}

void print_prim(Primary *prim){
    if(prim != NULL){
        if(!strcmp(prim->type, "bool")){
            if(prim->val == 1)
                printf("true");
            else
                printf("false");
        }
        else if(!strcmp(prim->type, "int"))
            printf("%d", prim->val);
    }
}


void print_binOp(BinOp *b){
    if(b != NULL){
        switch(b->op){
            case '!':
                printf("~");
                print_exp(b->lhs);
                break;
            case '|':
                print_exp(b->lhs);
                printf(" || ");
                print_exp(b->rhs);
                break;

            case '&':
                print_exp(b->lhs);
                printf(" && ");
                print_exp(b->rhs);
                break;

            case '=':
                print_exp(b->lhs);
                printf(" == ");
                print_exp(b->rhs);
                break;

            case '(':
                print_exp(b->lhs);
                printf(" <= ");
                print_exp(b->rhs);
                break;

            case ')':
                print_exp(b->lhs);
                printf(" >= ");
                print_exp(b->rhs);
                break;

            default:
                print_exp(b->lhs);
                printf(" %c ", b->op);
                print_exp(b->rhs);
                break;

        }
    }
}

void print_assignment(Assignment *assgn, int tabs){
    if(assgn != NULL){
        print_tabs(tabs);
        print_var(assgn->lhs);
        printf(" = ");
        print_exp(assgn->rhs);
    }
}

void print_var(Var *var){
    if(var != NULL){
        if(var->utype == ID_VAR)
            printf("%s", var->var_u->id);
        if(var->utype == OBJ_VAR)
            print_obj(var->var_u->obj);
    }
}

void print_obj(Object *obj){
    if(obj != NULL){
        if(obj->utype == NEW_OBJ)
            print_new(obj->obj_u->newObj);

        else if(obj->utype == METH_OBJ)
            print_methodInvoc(obj->obj_u->meth);

        else if(obj->utype == FIELD_OBJ)
            print_fieldAccess(obj->obj_u->field);
    }
}

void print_methodInvoc(MethodInvoc *minvok){
    if(minvok != NULL){
        print_var(minvok->obj);
        if(minvok->obj != NULL)
            printf(".");
        printf("%s(", minvok->mname);
        print_argList(minvok->args);
        printf(")");
    }
}

void print_fieldAccess(FieldAccess *faccess){
    if(faccess != NULL){
        print_var(faccess->obj);
        printf(".%s", faccess->fname);
    }
}

void print_new(New *n){
    if(n != NULL){
        printf("new %s", n->cname);
        printf("(");
        print_argList(n->args);
        printf(")");
    }
}

void print_argList(ArgList *args){
    if(args != NULL){
        print_exp(args->arg);
        if(args->next!=NULL)
            printf(", ");
        print_argList(args->next);
    }
}

void print_tabs(int tabs){
    for(int i=0; i<tabs; i++)
        printf("\t");
}


