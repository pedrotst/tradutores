#ifndef __ast_h
#define __ast_h

#include <stdio.h>
#include <stdlib.h>
#include <err.h>

#define TERM_T 1
#define EXP_T 2
#define LINE_T 3
#define OPNODE_T 4

typedef struct exp_s exp;

typedef union symbol_un {
    char op;
    int num;
}symbol_u;

typedef struct symbol_s{
    int tag;
    symbol_u u;
}symbol;

typedef struct term_s{
    int num;
}term;

typedef struct bin_op_s{
    char op;
    exp *lhs;
    term *rhs;
}bin_op;

union exp_u{
    term *t;
    bin_op *b_op;
};

struct exp_s{
    int tag;
    union exp_u expr;
};

exp* exp_alloc();
term* term_alloc();
bin_op* bin_alloc();
term* term_node(int n);

exp* exp_opnode(char op, exp* lhs, term* rhs);
exp* exp_termnode(term* t);
void print_exp(exp* a, int n);
void print_opnode(bin_op* b_op, int tabs);
void print_term(term* t, int tabs);

void destruct_tree(exp* a);
void print_symbol(symbol s);
void print_ntype(int n_type);

#endif
