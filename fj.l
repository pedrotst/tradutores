%{
#include <math.h>
#include <string.h>
#include "y.tab.h"

extern int count_lines, chars;
void parse_comment(char* yytext);
// extern YYSTYPE yylval;
extern int yywrap(void);

%}

DIGIT   [0-9]
NOPERATOR >>|<<|^|\*\*|\|
KEYWORD	int|bool|this|Object|class|new|true|false|return|super|extends|if|else|while
ID 	[A-z_][A-z_0-9]*

%%

\/\*([^*]|[\n]|[\r\n]|(\*+([^*/]|[\n][\r\n])))*\*+\/   {parse_comment(yytext);/*Comentario*/}
"//".*\n    {count_lines++; chars=0;/* Comentario */}



"int"       {chars+=strlen(yytext); return INT;}
"bool"      {chars+=strlen(yytext); return BOOL;}
"this"      {chars+=strlen(yytext); return THIS;}
"Object"    {chars+=strlen(yytext); return OBJECT;}
"new"       {chars+=strlen(yytext); return NEW;}
"class"     {chars+=strlen(yytext); return CLASS;}
"true"      {chars+=strlen(yytext); return TRUE;}
"false"     {chars+=strlen(yytext); return FALSE;}
"return"    {chars+=strlen(yytext); return RETURN;}
"super"     {chars+=strlen(yytext); return SUPER;}
"extends"   {chars+=strlen(yytext); return EXTENDS;}
"if"        {chars+=strlen(yytext); return IF;}
"else"      {chars+=strlen(yytext); return ELSE;}
"while"     {chars+=strlen(yytext); return WHILE;}

{ID}        {yylval.strs=strdup(yytext);chars+=strlen(yytext);return ID;}       
","         {chars+=strlen(yytext); return COMMA;} 
";"         {chars+=strlen(yytext); return SEMICOLON;}
"="         {chars+=strlen(yytext); return VAR_ATTRIBUITION;}
"\("        {chars+=strlen(yytext); return L_PAREN;}
"\)"        {chars+=strlen(yytext); return R_PAREN;}
"\{"        {chars+=strlen(yytext); return L_BRACK;}
"\}"        {chars+=strlen(yytext); return R_BRACK;}
"."         {chars+=strlen(yytext); return DOT;}
"+-*/"      {chars+=strlen(yytext); return yytext[0];}
"~"         {chars+=strlen(yytext); return NOT;}
"&&"        {chars+=strlen(yytext); return BAND;}
"||"        {chars+=strlen(yytext); return BOR;}
"=="        {chars+=strlen(yytext); return BEQ;}
">="        {chars+=strlen(yytext); return BGE;}
"<="        {chars+=strlen(yytext); return BLE;}
">"         {chars+=strlen(yytext); return BGT;}
"<"         {chars+=strlen(yytext); return BLT;}

{DIGIT}*    {yylval.num = atoi(yytext); chars+=strlen(yytext); return NUM;}
[+-;]        {return yytext[0];}
[ \t\r]+      {chars+=strlen(yytext);}
[\n]		{count_lines++;chars=0;}
.	        {printf("Line %d, %d: Character %s not recognized\n", count_lines, chars, yytext);chars+=strlen(yytext);}

%%

void parse_comment(char* yytext){
    int i; 
    for(i=0; i < strlen(yytext); i++){
        if (yytext[i]=='\n') count_lines++;
    }
}

// int yywrap (void) {return 1;}

//{KEYWORD}   {printf("KEYWORD: %s\n", yytext);chars+=strlen(yytext);}
/*
//".*\n        { }

tipo de comentário
regras de erro:
5 erros lexicos - ex 
falar a linha do erro
ler a espec
para compilar e rodar:
lex exemplo.lex && gcc lex.yy.c -lfl && ./a.out
 
}



main( argc, argv )
int argc;
char **argv;
    {
    ++argv, --argc;  // skip over program name
    if ( argc > 0 )
            yyin = fopen( argv[0], "r" );
    else
            yyin = stdin;

    yylex();
    }
*/

