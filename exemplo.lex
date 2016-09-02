/* scanner for a toy Pascal-like language */

%{
/* need this for the call to atof() below */
#include <math.h>
int contalinhas = 0;
%}

DIGIT   [0-9]
KEYWORD	int|bool|this|Object|true|false|return|super|extends|if|else|while
ID 	[A-z][A-z0-9]*
%%

{KEYWORD} {printf( "KEYWORD: %s\n", yytext);}
{ID} {printf( "ID: %s\n", yytext);}
{DIGIT}*{ID} {printf("Line %d: Malformed arithmethic expression\n", contalinhas);}
{DIGIT}*{ID} {printf("Line %d: IDs can't start with a digits\n", contalinhas);}
{DIGIT}*    {printf( "NUM: %s\n", yytext);}
","        {printf( "COMMA\n");}
";"        {printf( "SEMICOLON\n");}
"="        {printf( "VAR_ATTRIBUITION\n");}
"\("        {printf( "L_PAREN\n");}
"\)"        {printf( "R_PAREN\n");}
"\{"        {printf( "L_CURL_BRACKETS\n");}
"\}"        {printf( "R_CURL_BRACKETS\n");}
"."        {printf( "DOT\n");}
"+"        {printf( "PLUS\n");}
"-"        {printf( "MINUS\n");}
"*"        {printf( "MULT\n");}
"/"        {printf( "DIV\n");}
"**"        {printf( "POW\n");}
"~"        {printf( "NOT\n");}
"&&"        {printf( "BAND\n");}
"||"        {printf( "BOR\n");}
"=="        {printf( "BEQ\n");}
[ \t\n]+        {contalinhas++;/* Caracteres vazios :)*/}
.	{printf("Line %d: Character %s not recognized\n", yytext, contalinhas);

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
