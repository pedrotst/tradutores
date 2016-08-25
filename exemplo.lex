/* scanner for a toy Pascal-like language */

%{
/* need this for the call to atof() below */
#include <math.h>
%}

DIGIT   [0-9]
KEY 	int|bool|this|Object|true|false|return|super|extends|if|else|while
ID 	[a-zA-Z][a-zA-Z0-9]*
%%

{KEY} { printf( "KEYWORD: %s\n", yytext); }
{ID} { printf( "ID: %s\n", yytext); }
{DIGIT}*    { printf( "NUM: %s\n", yytext); }
","        { printf( "COMMA\n"); }
";"        { printf( "SEMICOLON\n"); }
"="        { printf( "VAR_ATTRIBUITION\n"); }
"\("        {printf( "L_PAREN\n");}
"\)"        {printf( "R_PAREN\n");}
"\{"        {printf( "L_CURL_BRACKETS\n");}
"\}"        {printf( "R_CURL_BRACKETS\n");}
"."        {printf( "DOT\n");}
"+"        { printf( "PLUS\n"); }
"-"        { printf( "MINUS\n"); }
"*"        { printf( "MULT\n"); }
"/"        { printf( "DIV\n"); }
"**"        { printf( "POW\n"); }
"~"        { printf( "NOT\n"); }
"&&"        { printf( "BAND\n"); }
"||"        { printf( "BOR\n"); }
"=="        { printf( "BEQ\n"); }
[ \t\n]+        {/* Caracteres vazios :)*/ }
.	{ printf("Caracter nao especificado!\n");
/* 
tipo de comentário
regras de erro:
5 erros lexicos - ex 
falar a linha do erro
ler a espec
para compilar e rodar:
lex exemplo.lex && gcc lex.yy.c -lfl && ./a.out
*/ 
}
%%

main( argc, argv )
int argc;
char **argv;
    {
    ++argv, --argc;  /* skip over program name */
    if ( argc > 0 )
            yyin = fopen( argv[0], "r" );
    else
            yyin = stdin;

    yylex();
    }
