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

ClassDecl* classDecl_node(char *selfName, ClassName *superName, 
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


void print_program(Program* p){
    printf("entrei\n");
    ClassDecl *cdecl = p->classes;
    StmtList *stmt = p->stmts;
    while(cdecl != NULL){
        print_class(cdecl);
        cdecl = p->classes->nextClass;
    }
    while(stmt != NULL){
        print_stmt(stmt);
        stmt = p->stmts;
    }
}

void print_class(ClassDecl *c){
    ClassDecl *cdecl = c;
    while(cdecl != NULL){
        // printf("class %s extends %s{", cdecl->selfName, cdecl->superName);
        printf("class %s extends {", cdecl->selfName);
        //printf("\tfields: {");
        // print_var(cdecl->varDecls);
        printf("}");
        // print_constructor();
        // print_mdecl();
        cdecl = c->nextClass;
    }

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
