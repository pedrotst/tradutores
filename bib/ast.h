#ifndef __ast_h
#define __ast_h

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define TERM_T 1
#define EXP_T 2
#define LINE_T 3

typedef union symbol_un {
    char op;
    int num;
}symbol_u;

typedef struct symbol_s{
    int tag;
    symbol_u u;
}symbol;

typedef struct ast_s {
    symbol sym;
    int node_type;
    struct ast_s *left;
    struct ast_s *right;
}ast;

ast* node_alloc();
ast* node(symbol s, int node_t, ast* l, ast* r);
void destruct_tree(ast* a);
ast* leafnode(symbol s);
void print_ast(ast* a, int n);
void print_symbol(symbol s);
void print_ntype(int n_type);



#endif
