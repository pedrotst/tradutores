%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>

int count_lines = 1, chars = 0;

void yyerror(const char*);
int yywrap();
int yylex(void);
// extern int yylval;
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
ast* leafnode(symbol s);
void print_ast(ast* a, int n);
void print_symbol(symbol s);
void print_ntype(int n_type);

%}


%start line
%union {int num; struct ast_s* ast_v;}

%token ID NUM COMMA SEMICOLON VAR_ATTRIBUITION L_PAREN R_PAREN
%token L_CURL_BRACKETS R_CURL_BRACKETS DOT PLUS MINUS MULT DIV
%token NOT BAND BOR BEQ BGE BLE BGT BL PRINT

%type <num> NUM 
%type <ast_v> line exp term

%%
line    
: exp ';'
    { print_ast($1, 0);
    //printf("line %d:%d - val %d\n", count_lines, chars, $1);
    }

exp     
: term                  
    {symbol s; s.tag = -1;
    $$ = node (s, EXP_T, $1, NULL);}
| exp '+' term          
    {symbol s; s.tag = 1; s.u.op='+';
    $$ = node (s, EXP_T, $1 , $3);}
| exp '-' term          
    {symbol s; s.tag = 1; s.u.op='-';
    $$ = node (s, EXP_T, $1 , $3);}
;

term    
: NUM                   
    {symbol s; s.tag = 0; s.u.num=$1;
    $$ = leafnode(s);}
;

%%

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



void yyerror(const char *str)
{
    fprintf(stderr,"error: %s\n",str);
}

int yywrap() {return 1;};

int main()
{
    yyparse();
    return 0;
} 


