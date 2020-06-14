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

"mkfs"          { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return mkfs_tk; }
"login"         { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return login_tk; }
"mkgrp"         { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return mkgrp_tk; }
"rmgrp"         { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return rmgrp_tk; }
"mkusr"         { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return mkusr_tk; }
"rmusr"         { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return rmusr_tk; }
"chmod"         { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return chmod_tk; }
"mkfile"        { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return mkfile_tk; }
"cat"           { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return cat_tk; }
"rem"           { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return rem_tk; }
"edit"          { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return edit_tk; }
"ren"           { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return ren_tk; }
"mkdir"         { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return mkdir_tk; }
"cp"            { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return cp_tk; }
"mv"            { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return mv_tk; }
"find"          { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return find_tk; }
"chown"         { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return chown_tk; }
"chgrp"         { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return chgrp_tk; }
"pause"         { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return pause_tk; }


"-size"         { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return size_tk; }
"-fit"          { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return fit_tk; }
"-unit"         { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return unit_tk; }
"-path"         { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return path_tk; }
"-type"         { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return type_tk; }
"-delete"       { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return delete_tk; }
"-name"         { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return name_tk; }
"-add"          { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return add_tk; }
"-id"           { columna=columna+ anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return idk_tk; }

"-usr"          { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return ust_tk; }
"-pwd"          { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return pwd_tk; }
"-grp"          { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return grp_tk; }
"-ugo"          { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return ugo_tk; }
"-r"            { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return r_tk; }
"-p"            { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return p_tk; }
"-cont"         { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return cont_tk; }
"-file"         { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return file_tk; }
"-dest"         { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return dest_tk; }
"-ruta"         { columna=columna+anterior; anterior=strlen(yytext); strcpy(yylval.TEXT,yytext); return ruta_tk; }


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
