%{
#include <math.h>
#include <string.h>
#include "y.tab.h"

void parse_comment(char* yytext);
extern int yywrap(void);
int l_errs = 0;
/* 
extern YYSTYPE yylval;
"super"     {chars+=strlen(yytext); return SUPER;}
"Object"    {chars+=strlen(yytext); return OBJECT;}
*/


int yycolumn = 1;

// Macro necessary for location of a token
#define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng; \

%}
%option yylineno

DIGIT   [0-9]
NOPERATOR >>|<<|^|\*\*|\|
KEYWORD	int|bool|this|Object|class|new|true|false|return|super|extends|if|else|while
ID 	[A-z_][A-z_0-9]*
SEP	\(|\)|;|,|=|\{|\}|\*|\/|\+|-
%%

\/\*([^*]|[\n]|[\r\n]|(\*+([^*/]|[\n][\r\n])))*\*+\/   {parse_comment(yytext);/*Comentario*/}
"//".*\n    { yycolumn = 1; /* Comentario */}



"int"       {yylval.strs=strdup(yytext);return INT;}
"bool"      {yylval.strs=strdup(yytext);return BOOL;}
"new"       {return NEW;}
"class"     {return CLASS;}
"true"      {return TRUE;}
"false"     {return FALSE;}
"return"    {return RETURN;}
"extends"   {return EXTENDS;}
"if"        {return IF;}
"else"      {return ELSE;}
"while"     {return WHILE;}

{ID}        {yylval.strs=strdup(yytext);return ID;}       
{SEP}       {return yytext[0];}
"."         {return DOT;}
"~"         {return NOT;}
"&&"        {return BAND;}
"||"        {return BOR;}
"=="        {return BEQ;}
">="        {return BGE;}
"<="        {return BLE;}
">"         {return BGT;}
"<"         {return BLT;}

{DIGIT}*    {yylval.num = atoi(yytext); return NUM;}
[ \t\r]+    { }
[\n]		{yycolumn = 1;}
.	        {printf("Line %d, %d: Character %s not recognized\n", yylineno, yycolumn, yytext); l_errs++;}

%%

void parse_comment(char* yytext){
    int i; 
    for(i=0; i < strlen(yytext); i++){
        if (yytext[i]=='\n'){
            yylloc.first_line = yylloc.last_line = yylineno;
            yycolumn = 1;
        }
    }
}


