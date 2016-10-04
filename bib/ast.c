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
        ClassMembers *cMembers, ClassDecl *head){

    ClassDecl *c = (ClassDecl*) malloc(sizeof(ClassDecl));
    if(c == NULL){
        err(1, "Could not allocate memory for Class");
        exit(1);
    }
    c->selfName = selfName;
    c->superName = superName;
    c->cMembers = cMembers;
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

VarDecl* varDecl_node(char *type, char *id, IdList *ids){
    VarDecl* v = (VarDecl*)malloc(sizeof(VarDecl));
    v->type = type;
    v->id = id;
    v->idList = ids;
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
    FormalArgs *fargs, StmtList *stmtList){

    FunctionDecl* f_decl = (FunctionDecl*)malloc(sizeof(FunctionDecl));

    f_decl->name = name;
    f_decl->type = type;
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

Exp* exp_node(tag utype, Var *var, BinOp *binOp,
        Assignment *assgn, Exp *parenthesis){
    Exp *e = (Exp*)malloc(sizeof(Exp));
    Exp_u *exp = (Exp_u*)malloc(sizeof(Exp_u));
    e->utype = utype;
    if(utype == VAR_DECL)
        exp->var = var;

    e->exp_u = exp;
    return e;
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
    if(fargs != NULL)
        printf("%s %s", fargs->type, fargs->name);
    fargs = fargs->next;

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
    // print_exp();
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
    // print_exp(w->cond);
    printf("{\n");
    print_stmt(w->loop, tabs+1);
    printf("}\n");
}

void print_return(Exp *e, int tabs){
    print_tabs(tabs);
    printf("return ");
    // print_exp();
}

void print_exp(Exp *e, int tabs){
}

void print_assignment(Assignment *assgn, int tabs){
    if(assgn != NULL){
        print_tabs(tabs);
        print_var(assgn->lhs);
        printf(" = ");
        // print_exp(assgn->rhs);
    }
}

void print_var(Var *var){
    if(var != NULL){
        if(var->utype == ID_VAR)
            printf("%s", var->var_u->id);
    }
}

void print_tabs(int tabs){
    for(int i=0; i<tabs; i++)
        printf("\t");
}
