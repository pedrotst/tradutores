%{
#include <math.h>
#include <string.h>
#include "y.tab.h"

extern int count_lines, ch_end, ch_begin;
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
"//".*\n    {count_lines++; ch_end=0;/* Comentario */}



"int"       {yylval.strs=strdup(yytext);ch_end+=yyleng; return INT;}
"bool"      {yylval.strs=strdup(yytext);ch_end+=yyleng; return BOOL;}
"new"       {ch_end+=yyleng; return NEW;}
"class"     {ch_end+=yyleng; return CLASS;}
"true"      {ch_end+=yyleng; return TRUE;}
"false"     {ch_end+=yyleng; return FALSE;}
"return"    {ch_end+=yyleng; return RETURN;}
"extends"   {ch_end+=yyleng; return EXTENDS;}
"if"        {ch_end+=yyleng; return IF;}
"else"      {ch_end+=yyleng; return ELSE;}
"while"     {ch_end+=yyleng; return WHILE;}

{ID}        {yylval.strs=strdup(yytext);ch_begin=ch_end;ch_end+=yyleng;return ID;}       
{SEP}  {ch_end+=yyleng; return yytext[0];}
"."         {ch_end+=yyleng; return DOT;}
"~"         {ch_end+=yyleng; return NOT;}
"&&"        {ch_end+=yyleng; return BAND;}
"||"        {ch_end+=yyleng; return BOR;}
"=="        {ch_end+=yyleng; return BEQ;}
">="        {ch_end+=yyleng; return BGE;}
"<="        {ch_end+=yyleng; return BLE;}
">"         {ch_end+=yyleng; return BGT;}
"<"         {ch_end+=yyleng; return BLT;}

{DIGIT}*    {yylval.num = atoi(yytext); ch_end+=yyleng; return NUM;}
[ \t\r]+      {ch_end+=yyleng;}
[\n]		{count_lines++;ch_end=0;}
.	        {printf("Line %d, %d: Character %s not recognized\n", count_lines, ch_end, yytext);ch_end+=yyleng;}

%%

void parse_comment(char* yytext){
    int i; 
    for(i=0; i < strlen(yytext); i++){
        if (yytext[i]=='\n') count_lines++;
    }
}


