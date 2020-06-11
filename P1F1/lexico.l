%option case-insensitive
%option yylineno

%{
#include <cstdlib>
#include "parser.h"
#include <QString>
#include <stdio.h>
#include <iostream>
#include <nodoast.h>

int yylex();
int linea = 0;
int columna = 1;
int anterior = 0;
NodoAST *root;

%}


%option header-file="scanner.h"
%option outfile="scanner.cpp"
%option noyywrap
%option nounput

let         [a-zñA-ZÑ]+
num         [0-9]+
nun         "-"[0-9]+

id          {let}({let}|{num}|"_")*
rut         ("/"{let}({let}|{num}|"_")*)+"."{let}({let}|{num}|"_")*|"\""("/"({let}({let}|{num}|"_"|" ")+)*)+"."{let}({let}|{num}|"_")*"\""
cad         "\""[^"\""]*"\""
com         "#"[^"\n"]*


%%
INITIAL
{


"mkdisk"        { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return mkdisk_tk; }
"rmdisk"        { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return rmdisk_tk; }
"fdisk"         { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return fdisk_tk; }
"mount"         { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return mount_tk; }
"unmount"       { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return unmount_tk; }
"exec"          { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return exec_tk; }
"rep"           { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return rep_tk; }

"-size"         { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return size_tk; }
"-fit"          { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return fit_tk; }
"-unit"         { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return unit_tk; }
"-path"         { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return path_tk; }
"-type"         { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return type_tk; }
"-delete"       { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return delete_tk; }
"-name"         { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return name_tk; }
"-add"          { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return add_tk; }
"-id"           { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return idk_tk; }

"="             { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return igual_tk; }

{com}           { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return comentario_tk; }
{id}            { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return id_tk; }
{rut}           { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return rut_tk; }
{cad}           { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return cad_tk; }
{num}           { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return num_tk; }
{nun}           { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return nun_tk; }


"\t"            { strcpy(yylval.TEXT,yytext); columna=26; }
[[:blank:]]     { columna++; }
"\n"|"\r"       { linea++; columna=0; anterior=0; }
.               { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); std::cout<<yytext<<"Error Lexico-> Linea:"<<linea<<" Columna:"<<columna<<std::endl; }


}


%%
