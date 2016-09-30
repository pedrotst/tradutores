#include "ast.h"

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
/*


ast* leafnode(symbol s){
    ast *a = node_alloc();
    a->node_type = TERM_T;
    a->sym = s;
    a->left = NULL;
    a ->right = NULL;
    return a;
}



void destruct_tree(ast* a){
    ast *l, *r;
    if(a!=NULL){
        free(a);
        l = a->left;
        r = a->right;
        destruct_tree(l);
        destruct_tree(r);
    }
}
void print_ntype(int n_type){
    switch(n_type){
        case TERM_T:
            printf("TERM: ");
            break;
        case EXP_T:
            printf("EXP: ");
            break;
        case LINE_T:
            printf("LINE: ");
            break;
        default:
            break;
    }
}

void print_symbol(symbol s){
    switch(s.tag){
        case 0:
        printf("%d", s.u.num);
        break;
        case 1:
        printf("%c", s.u.op);
        break;
        default:
        break;
    }
}
*/

