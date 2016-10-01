/* scanner for a toy Pascal-like language */

%{
/* need this for the call to atof() below */
#include <math.h>
#include <string.h>
int count_lines = 0, chars = 0;
%}

DIGIT   [0-9]
NOPERATOR >>|<<|^|\*\*|\|
ID 	[A-z_][A-z_0-9]*

%%

{DIGIT}+{ID}|{DIGIT}+{KEYWORD} {printf("Line %d,%d: Malformed arithmethic expression\n", count_lines, chars); }
{NOPERATOR}        {printf("Line %d,%d: Operator %s Not Supported\n", count_lines, chars, yytext);chars+=strlen(yytext);}
{ID} {printf("ID: %s\n", yytext);chars+=strlen(yytext);}
{DIGIT}*    {printf("NUM: %s\n", yytext);chars+=strlen(yytext);}



","        {printf("COMMA\n");chars+=strlen(yytext);}
";"        {printf("SEMICOLON\n");chars+=strlen(yytext);}
"="        {printf("VAR_ATTRIBUITION\n");chars+=strlen(yytext);}
"\("        {printf("L_PAREN\n");chars+=strlen(yytext);}
"\)"        {printf("R_PAREN\n");chars+=strlen(yytext);}
"\{"        {printf("L_CURL_BRACKETS\n");chars+=strlen(yytext);}
"\}"        {printf("R_CURL_BRACKETS\n");chars+=strlen(yytext);}
"."        {printf("DOT\n");chars+=strlen(yytext);}
"+"        {printf("PLUS\n");chars+=strlen(yytext);}
"-"        {printf("MINUS\n");chars+=strlen(yytext);}
"*"        {printf("MULT\n");chars+=strlen(yytext);}
"/"        {printf("DIV\n");chars+=strlen(yytext);}
"~"        {printf("NOT\n");chars+=strlen(yytext);}
"&&"        {printf("BAND\n");chars+=strlen(yytext);}
"||"        {printf("BOR\n");chars+=strlen(yytext);}
"=="        {printf("BEQ\n");chars+=strlen(yytext);}
">="        {printf("BGE\n");chars+=strlen(yytext);}
"<="        {printf("BLE\n");chars+=strlen(yytext);}
">"        {printf("BGT\n");chars+=strlen(yytext);}
"<"        {printf("BLT\n");chars+=strlen(yytext);}
[ \t\n]+        {chars+=strlen(yytext);/* Caracteres vazios :)*/}
[\n]+		{count_lines++;chars=0;}
.	{printf("Line %d: Character %s not recognized\n", count_lines, yytext);chars+=strlen(yytext);

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
