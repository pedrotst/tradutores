%{
#include <math.h>
#include <string.h>
int count_lines = 1, chars = 0;

void parse_comment(char* yytext);
%}

DIGIT   [0-9]
NOPERATOR >>|<<|^|\*\*|\|
KEYWORD	int|bool|this|Object|class|new|true|false|return|super|extends|if|else|while
ID 	[A-z_][A-z_0-9]*

%%

\/\*([^*]|[\n]|[\r\n]|(\*+([^*/]|[\n][\r\n])))*\*+\/   {parse_comment(yytext);/*Comentario*/}
"//".*\n {count_lines++; chars=0;/* Comentario */}
{DIGIT}+{ID}|{DIGIT}+{KEYWORD} {printf("Line %d,%d: Malformed arithmethic expression\n", count_lines, chars); chars+=strlen(yytext);}
{NOPERATOR} {printf("Line %d,%d: Operator %s Not Supported\n", count_lines, chars, yytext);chars+=strlen(yytext);chars+=strlen(yytext);}
{KEYWORD}   {printf("KEYWORD: %s\n", yytext);chars+=strlen(yytext);}
{ID}        {printf("ID: %s\n", yytext);chars+=strlen(yytext);}
{DIGIT}*    {printf("NUM: %s\n", yytext);chars+=strlen(yytext);}
","         {printf("COMMA\n");chars+=strlen(yytext);}
";"         {printf("SEMICOLON\n");chars+=strlen(yytext);}
"="         {printf("VAR_ATTRIBUITION\n");chars+=strlen(yytext);}
"\("        {printf("L_PAREN\n");chars+=strlen(yytext);}
"\)"        {printf("R_PAREN\n");chars+=strlen(yytext);}
"\{"        {printf("L_CURL_BRACKETS\n");chars+=strlen(yytext);}
"\}"        {printf("R_CURL_BRACKETS\n");chars+=strlen(yytext);}
"."         {printf("DOT\n");chars+=strlen(yytext);}
"+"         {printf("PLUS\n");chars+=strlen(yytext);}
"-"         {printf("MINUS\n");chars+=strlen(yytext);}
"*"         {printf("MULT\n");chars+=strlen(yytext);}
"/"         {printf("DIV\n");chars+=strlen(yytext);}
"~"         {printf("NOT\n");chars+=strlen(yytext);}
"&&"        {printf("BAND\n");chars+=strlen(yytext);}
"||"        {printf("BOR\n");chars+=strlen(yytext);}
"=="        {printf("BEQ\n");chars+=strlen(yytext);}
">="        {printf("BGE\n");chars+=strlen(yytext);}
"<="        {printf("BLE\n");chars+=strlen(yytext);}
">"         {printf("BGT\n");chars+=strlen(yytext);}
"<"         {printf("BLT\n");chars+=strlen(yytext);}
[ \t\r]+      {chars+=strlen(yytext);/* Caracteres vazios :)*/}
[\n]		{count_lines++;chars=0;}
.	        {printf("Line %d, %d: Character %s not recognized\n", count_lines, chars, yytext);chars+=strlen(yytext);

//".*\n        {/*Comentario*/}
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
void parse_comment(char* yytext){
    int i; 
    for(i=0; i < strlen(yytext); i++){
        if (yytext[i]=='\n') count_lines++;
    }
}

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
