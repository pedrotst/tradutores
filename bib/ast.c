#include "ast.h"

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
        ClassMembers *cMembers, ClassDecl *head, ClassTable **ctable){

    ClassDecl *c = (ClassDecl*) malloc(sizeof(ClassDecl));
    if(c == NULL){
        err(1, "Could not allocate memory for Class");
        exit(1);
    }
    c->selfName = selfName;
    c->superName = superName;
    c->cMembers = cMembers;
    c->next= NULL;

    ClassTable *ct_aux, *ct_node = (ClassTable*)malloc(sizeof(ClassTable));
    ct_node->selfName = selfName;
    ct_node->superName = superName;
    ct_node->next = NULL;

    if (*ctable==NULL)
        *ctable = ct_node;
    else{
        ct_aux = *ctable;
        while(ct_aux->next!=NULL)
            ct_aux = ct_aux->next;
        ct_aux->next = ct_node;
    }

    if(head == NULL)
        return c;

    ClassDecl *current = head;
    while(current->next != NULL){
        current = current->next;
    }

    current->next = c;

 

    return head;
}

ClassMembers* classMembers_node(ClassMember *member, ClassMembers *head){

    ClassMembers *c = (ClassMembers*) malloc(sizeof(ClassMembers));
    c->member = member;
    c->next = NULL;

    if(head == NULL)
        return c;

    ClassMembers *current = head;
    while(current->next != NULL){
        current = current->next;
    }

    current->next = c;

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

VarDecl* varDecl_node(char *type, char *id, IdList *ids, 
        VariableTable **vtable){
    VarDecl* v = (VarDecl*)malloc(sizeof(VarDecl));
    v->type = type;
    v->id = id;
    v->idList = ids;
    
    VariableTable *vt_aux, *vt_next, *vt_node = (VariableTable*)malloc(sizeof(VariableTable));
    vt_node->type = type;
    vt_node->name = id;
    vt_node->next = NULL;
    vt_aux = vt_node;

    IdList *vnames = v->idList;
    while(vnames != NULL){
        vt_next = (VariableTable*)malloc(sizeof(VariableTable));
        vt_next->name = vnames->id;
        vt_next->type = type;
        vt_next->next = NULL;

        vt_aux->next = vt_next;
        vt_aux = vt_next;

        vnames = vnames->next;
    }

    if (*vtable==NULL)
        *vtable = vt_node;
    else{
        vt_aux = *vtable;
        while(vt_aux->next!=NULL)
            vt_aux = vt_aux->next;
        vt_aux->next = vt_node;
    }

    return v;
}

StmtList* stmtList_node(Stmt *stmt, StmtList *head){
    StmtList* stmt_list = (StmtList*)malloc(sizeof(StmtList));
    stmt_list->stmt = stmt;
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

IdList* idList_node(char *id, IdList *head){
    IdList* id_list = (IdList*)malloc(sizeof(IdList));
    id_list->id = id;
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

FormalArgs* formalArgs_node(char *type, char *name, FormalArgs *head){
    FormalArgs* fargs = (FormalArgs*)malloc(sizeof(FormalArgs));
    fargs->type = type;
    fargs->name = name;
    fargs->next = NULL;

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
    FormalArgs *fargs, StmtList *stmtList, FunctionTable **ftable){

    FunctionDecl* f_decl = (FunctionDecl*)malloc(sizeof(FunctionDecl));

    f_decl->name = name;
    f_decl->type = type;
    f_decl->fargs = fargs;
    f_decl->stmts = stmtList;
 
    FunctionTable *ft_aux, *ft_node = (FunctionTable*)malloc(sizeof(FunctionTable));
    ft_node->type = type;
    ft_node->name = name;
    ft_node->fargs = fargs;
    ft_node->next = NULL;
    ft_aux = ft_node;

    if (*ftable==NULL)
        *ftable = ft_node;
    else{
        ft_aux = *ftable;
        while(ft_aux->next!=NULL)
            ft_aux = ft_aux->next;
        ft_aux->next = ft_node;
    }

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

Var* var_node(tag utype, char *id, Object *obj){
    Var *v = (Var*)malloc(sizeof(Var));
    Var_u *var = (Var_u*)malloc(sizeof(Var_u));
    v->utype = utype;
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
        Exp *parenthesis, Primary *primary){
    Exp *e = (Exp*)malloc(sizeof(Exp));
    Exp_u *exp = (Exp_u*)malloc(sizeof(Exp_u));
    e->utype = utype;
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

Primary* primary_node(tag type, int val){
    Primary *primary = (Primary*)malloc(sizeof(Primary));
    primary->type = type;
    primary->val = val;
    return primary;
}


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

void print_classMembers(ClassMembers *cmembers){
    ClassMember *cmember;
    while(cmembers != NULL){
        cmember = cmembers->member;
        if(cmember != NULL){
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
        }
        cmembers = cmembers->next;
    }
}

void print_varDecl(VarDecl *varDecls, int tabs){
    print_tabs(tabs);
    printf("%s %s", varDecls->type, varDecls->id);
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
        if(prim->type == BOOL_PRIM){
            if(prim->val == 1)
                printf("true");
            else
                printf("false");
        }
        else if(prim->type == INT_PRIM)
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
