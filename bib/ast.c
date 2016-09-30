#include "ast.h"

ast* node_alloc(){
    ast *a = (ast*)malloc(sizeof(ast));
    if(a == NULL){
        err(1, "Could not allocate memory for the tree");
        exit(1);
    }
    a->left = NULL;
    a->right = NULL;

    return a;
}

ast* node(symbol s, int node_t, ast* l, ast* r){
    ast *a = node_alloc();
    a->node_type = node_t;
    a->sym = s;
    a->left = l;
    a->right = r;
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

ast* leafnode(symbol s){
    ast *a = node_alloc();
    a->node_type = TERM_T;
    a->sym = s;
    a->left = NULL;
    a ->right = NULL;
    return a;
}

void print_ast(ast* a, int tabs){
    for(int i=0; i<tabs; i++)
        printf("\t");
    print_ntype(a->node_type);
    print_symbol(a->sym);
    printf("{\n");
    if(a->left != NULL)
        print_ast(a->left, tabs+1);
    if(a->right != NULL)
        print_ast(a->right, tabs+1);
    for(int i=0; i<tabs; i++)
        printf("\t");
    printf("}\n");
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


