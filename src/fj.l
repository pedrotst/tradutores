%{
#include <math.h>
#include <string.h>
#include "y.tab.h"

extern int count_lines, chars;
void parse_comment(char* yytext);
extern int yywrap(void);

/* 
extern YYSTYPE yylval;
"super"     {chars+=strlen(yytext); return SUPER;}
"Object"    {chars+=strlen(yytext); return OBJECT;}
*/


%}

DIGIT   [0-9]
NOPERATOR >>|<<|^|\*\*|\|
KEYWORD	int|bool|this|Object|class|new|true|false|return|super|extends|if|else|while
ID 	[A-z_][A-z_0-9]*
SEP	\(|\)|;|,|=|\{|\}|\*|\/|\+|-
%%

\/\*([^*]|[\n]|[\r\n]|(\*+([^*/]|[\n][\r\n])))*\*+\/   {parse_comment(yytext);/*Comentario*/}
"//".*\n    {count_lines++; chars=0;/* Comentario */}



"int"       {yylval.strs=strdup(yytext);chars+=strlen(yytext); return INT;}
"bool"      {yylval.strs=strdup(yytext);chars+=strlen(yytext); return BOOL;}
"new"       {chars+=strlen(yytext); return NEW;}
"class"     {chars+=strlen(yytext); return CLASS;}
"true"      {chars+=strlen(yytext); return TRUE;}
"false"     {chars+=strlen(yytext); return FALSE;}
"return"    {chars+=strlen(yytext); return RETURN;}
"extends"   {chars+=strlen(yytext); return EXTENDS;}
"if"        {chars+=strlen(yytext); return IF;}
"else"      {chars+=strlen(yytext); return ELSE;}
"while"     {chars+=strlen(yytext); return WHILE;}

{ID}        {yylval.strs=strdup(yytext);chars+=strlen(yytext);return ID;}       
{SEP}  {chars+=strlen(yytext); return yytext[0];}
"."         {chars+=strlen(yytext); return DOT;}
"~"         {chars+=strlen(yytext); return NOT;}
"&&"        {chars+=strlen(yytext); return BAND;}
"||"        {chars+=strlen(yytext); return BOR;}
"=="        {chars+=strlen(yytext); return BEQ;}
">="        {chars+=strlen(yytext); return BGE;}
"<="        {chars+=strlen(yytext); return BLE;}
">"         {chars+=strlen(yytext); return BGT;}
"<"         {chars+=strlen(yytext); return BLT;}

{DIGIT}*    {yylval.num = atoi(yytext); chars+=strlen(yytext); return NUM;}
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


