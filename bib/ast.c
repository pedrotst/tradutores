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
        WhileStmt *whileStmt, ReturnStmt *returnStmt){
    Stmt *stmt = (Stmt*)malloc(sizeof(Stmt));
    Stmt_u *stmt_u = (Stmt_u*)malloc(sizeof(Stmt_u));

    switch(utype){
        case VAR_DECL:
            stmt_u->varDecl = varDecl;
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

void print_program(Program* p){
    print_class(p->classes);
    print_stmt(p->stmts);
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
    for(int i=0; i<tabs; i++)
        printf("\t");
    printf("%s %s", varDecls->type, varDecls->id);
    print_idList(varDecls->idList);
    printf(";\n");
}

void print_constrDecl(ConstrDecl *constrDecl){
    if(constrDecl != NULL){
        printf("\t");
        printf("%s (", constrDecl->name);
        print_fargs(constrDecl->fargs);
        printf(") {");
        // print_stmtList();
        printf("}\n");
    }
}

void print_funDecl(FunctionDecl *funDecl){
    if(funDecl != NULL){
        printf("\t");
        printf("%s ", funDecl->type);
        printf("%s (", funDecl->name);
        print_fargs(funDecl->fargs);
        printf(") {");
        // print_stmtList();
        printf("}\n");
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

void print_stmt(StmtList *stmts){
    Stmt *stmt;
    while(stmts != NULL){
        stmt = stmts->stmt;
        switch(stmt->utype){
            case VAR_DECL:
                print_varDecl(stmt->stmt_u->varDecl, 0);
                break;
            default:
            break;
        }
        stmts = stmts->next;
    }

}

