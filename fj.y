%{
#include <stdio.h>
#include <string.h>

int count_lines = 1, chars = 0;

void yyerror(const char*);
int yywrap();
int yylex(void);
// extern int yylval;
#define TERM_T 0;
#define EXP_T 1;
#define LINE_T 2;

typedef union symbol_u {
    char op;
    int num;
}symbol;

typedef struct ast_s {
    symbol sym;
    int opToken;
    ast *left;
    ast *right;
}ast;

ast* node_alloc(){
ast* node(sym, int, ast*, ast*);
ast* leafnode(sym);
void print_ast(ast*, int);

%}


%start line
%union {int num;}

%token ID NUM COMMA SEMICOLON VAR_ATTRIBUITION L_PAREN R_PAREN
%token L_CURL_BRACKETS R_CURL_BRACKETS DOT PLUS MINUS MULT DIV
%token NOT BAND BOR BEQ BGE BLE BGT BL PRINT

%type <num> NUM
%type <ast> line exp term

%%
line    
: exp ';'         
    { print_ast($1, 0);
    //printf("line %d:%d - val %d\n", count_lines, chars, $1);
    }

exp     
: term                  {$$ = node (-1, TERM_T, $1, NULL);}
| exp '+' term          {$$ = node ('+', EXP_T, $1 , $3);}
| exp '-' term          {$$ = node ('-', EXP_T, $1 , $3);}
;

term    
: NUM                   {$$ = leafnode($1);}
;

%%
ast* node_alloc(){
    ast *a = (ast*)malloc(sizeof(ast));
    if a == NULL{
        err(1, "Could not allocate memory for the tree");
        exit(1);
    }

    return a;
}
ast* node(symbol s, int op, ast* l, ast* r){
    ast *a = node_alloc();
    a->sym = s;
    a->opToken = op;
    a->left = l;
    a->right = r;
    return a;
}

ast* leafnode(symbol s){
    ast *a = node_alloc();
    a->sym = s;
    a->opToken = -1;
    a->left = NULL;
    a ->right = NULL;
    return a;
}

void print_ast(ast* a, int tabs){
    for(int i=0; i<tabs; i++)
        print("\t");
    print(" 

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


