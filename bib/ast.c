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
    ClassMembers *cMembers, ClassDecl *nextClass){

    ClassDecl *c = (ClassDecl*) malloc(sizeof(ClassDecl));
    if(c == NULL){
        err(1, "Could not allocate memory for Class");
        exit(1);
    }
    c->selfName = selfName;
    c->superName = superName;
    c->cMembers = cMembers;
    c->nextClass = nextClass;
    return c;
}

ClassMembers* classMember_node(union_tag utype, VarDecl *varDecls, 
    Constructor *constructors, MethodDecl *methodDecls, 
    ClassMembers *nextMember){

    ClassMembers *c = (ClassMembers*) malloc(sizeof(ClassMembers));
    if(c == NULL){
        err(1, "Could not allocate memory for ClassMember");
        exit(1);
    }
    ClassMembers_u *cmem = (ClassMembers_u*) malloc(sizeof(ClassMembers_u));
    switch(utype){
        case VAR_DECL:
            cmem->varDecls = varDecls;
            c->member = cmem;
        break;
        default:
            c->utype = utype;
        break;
    }
    c->nextMember = nextMember;

    return c;
}

VarDecl* varDecl_node(char *type, IdList *ids){
    VarDecl* v = (VarDecl*)malloc(sizeof(VarDecl*));
    v->type = type;
    v->idList = ids;
    return v;
}

IdList* idList_node(char *id, IdList *ids){
    IdList* id_list = (IdList*)malloc(sizeof(IdList*));
    id_list->id = id;
    id_list->nextId = ids;
    return id_list;
}


void print_program(Program* p){
    ClassDecl *cdecl = p->classes;
    StmtList *stmt = p->stmts;
    print_class(cdecl);
    cdecl = p->classes->nextClass;
    print_stmt(stmt);
    printf("\n");
}

void print_class(ClassDecl *c){
    ClassDecl *cdecl = c;
    while(cdecl != NULL){
        printf("class %s extends %s{\n", cdecl->selfName, cdecl->superName);
        // printf_fields("\tfields: {");
        print_classMembers(c->cMembers);
        printf("}\n");
        cdecl = cdecl->nextClass;
    }

}

void print_classMembers(ClassMembers *cmember){
   while(cmember != NULL){
       switch(cmember->utype){
           case VAR_DECL:
               print_varDecl(cmember->member->varDecls);
           break;
           default:
           break;
       }
   }
}

void print_varDecl(VarDecl *varDecls){
    printf("%s ", varDecls->type);
    print_idList(varDecls->idList);
}

void print_idList(IdList *ids){
    printf("%s", ids->id);
    ids = ids->nextId;
    while(ids!=NULL){
        printf(", %s", ids->id);
        ids = ids->nextId;
    }
    printf(";\n");
}

void print_stmt(StmtList *stmt){
}

/*
exp* exp_alloc(){
    exp *a = (exp*)malloc(sizeof(exp));
    if(a == NULL){
        err(1, "Could not allocate memory for exp");
        exit(1);
    }
    a->tag = -1;

    return a;
}

bin_op* bin_alloc(){
    bin_op *b_op = (bin_op*) malloc(sizeof(bin_op));
    if(b_op == NULL){
        err(1, "Could not allocate memory for exp");
        exit(1);
    }
    b_op->lhs = NULL;
    b_op->rhs = NULL;

    return b_op;
}

term* term_alloc(){
    term *t = (term*)malloc(sizeof(term));
    if(t == NULL){
        err(1, "Could not allocate memory for the tree");
        exit(1);
    }

    return t;
}

term* term_node(int n){
    term *t = term_alloc();
    t->num = n;
    return t;
}

exp* exp_opnode(char op, exp* lhs, term* rhs){
    exp *a = exp_alloc();
    bin_op *opbin = bin_alloc();
    opbin->op = op;
    opbin->lhs = lhs;
    opbin->rhs = rhs;

    a->tag = OPNODE_T;
    a->expr.b_op = opbin;

    return a;
}


exp* exp_termnode(term *t){
    exp *e = exp_alloc();

    e->tag = TERM_T;
    e->expr.t = t;
    return e;
}

void print_exp(exp* e, int tabs){
    for(int i=0; i<tabs; i++)
        printf("\t");
    printf("EXP: {\n");
    switch(e->tag){
        case TERM_T:
            print_term(e->expr.t, tabs+1);
            break;
        case OPNODE_T:
            print_opnode(e->expr.b_op, tabs+1);
            break;
        default:
            break;
    }
    for(int i=0; i<tabs; i++)
        printf("\t");
    printf("}\n");
}

void print_term(term* t, int tabs){
    for(int i=0; i<tabs; i++)
        printf("\t");
    printf("TERM: { %d }\n", t->num);
}

void print_opnode(bin_op* b_op, int tabs){
    for(int i=0; i<tabs; i++)
        printf("\t");
    printf("%c {\n", b_op->op);
    print_exp(b_op->lhs, tabs+1);
    print_term(b_op->rhs, tabs+1);
    for(int i=0; i<tabs; i++)
        printf("\t");
    printf("}\n");

}

*/
