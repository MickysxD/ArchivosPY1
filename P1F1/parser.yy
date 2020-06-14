%{
#include "scanner.h"//se importa el header del analisis sintactico
#include "qdebug.h"
#include <iostream>
#include <nodoast.h>
#include <qlist.h>

extern int yylineno; //linea actual donde se encuentra el parser (analisis lexico) lo maneja BISON
extern int columna; //columna actual donde se encuentra el parser (analisis lexico) lo maneja BISON
extern char *yytext; //lexema actual donde esta el parser (analisis lexico) lo maneja BISON
extern NodoAST *root; // Raiz del arbol

int yyerror(const char* mens)
{

    std::cout << mens <<" "<<yytext<< std::endl;
    return 0;
}
%}


//error-verbose si se especifica la opcion los errores sintacticos son especificados por BISON
%defines "parser.h"
%output "parser.cpp"
%error-verbose
%locations


%union{
//se especifican los tipo de valores para los no terminales y lo terminales
char TEXT [256];
class NodoAST *nodito;
}
/* mkfs_tk login_tk mkgrp_tk rmgrp_tk mkusr_tk rmusr_tk chmod_tk
   mkfile_tk cat_tk rem_tk edit_tk ren_tk mkdir_tk cp_tk mv_tk find_tk
   chown_tk chgrp_tk pause_tk

   usr_tk pwd_tk grp_tk ugo_tk r_tk p_tk cont_tk file_tk dest_tk ruta_tk */


/*TERMINALES*/
//Primera fase
%token<TEXT> mkdisk_tk;
%token<TEXT> rmdisk_tk;
%token<TEXT> fdisk_tk;
%token<TEXT> mount_tk;
%token<TEXT> unmount_tk;
%token<TEXT> exec_tk;
%token<TEXT> rep_tk;

//Segunda fase
%token<TEXT> mkfs_tk;
%token<TEXT> login_tk;
%token<TEXT> mkgrp_tk;
%token<TEXT> rmgrp_tk;
%token<TEXT> mkusr_tk;
%token<TEXT> rmusr_tk;
%token<TEXT> chmod_tk;
%token<TEXT> mkfile_tk;
%token<TEXT> cat_tk;
%token<TEXT> rem_tk;
%token<TEXT> edit_tk;
%token<TEXT> ren_tk;
%token<TEXT> mkdir_tk;
%token<TEXT> cp_tk;
%token<TEXT> mv_tk;
%token<TEXT> find_tk;
%token<TEXT> chown_tk;
%token<TEXT> chgrp_tk;
%token<TEXT> pause_tk;


//Primera fase
%token<TEXT> size_tk;
%token<TEXT> fit_tk;
%token<TEXT> unit_tk;
%token<TEXT> path_tk;
%token<TEXT> type_tk;
%token<TEXT> delete_tk;
%token<TEXT> name_tk;
%token<TEXT> add_tk;
%token<TEXT> idk_tk;
%token<TEXT> igual_tk;
%token<TEXT> comentario_tk;
%token<TEXT> id_tk;
%token<TEXT> rut_tk;
%token<TEXT> cad_tk;
%token<TEXT> num_tk;
%token<TEXT> nun_tk;

//Segunda fase
%token<TEXT> usr_tk;
%token<TEXT> pwd_tk;
%token<TEXT> grp_tk;
%token<TEXT> ugo_tk;
%token<TEXT> r_tk;
%token<TEXT> p_tk;
%token<TEXT> cont_tk;
%token<TEXT> file_tk;
%token<TEXT> dest_tk;
%token<TEXT> ruta_tk;


/*NO TERMINALES*/
//Primera fase
%type<nodito> INICIO;
%type<nodito> LISTA;
%type<nodito> TIPOS;
%type<nodito> MKDISK;
%type<nodito> MKDISK_L;
%type<nodito> MKDISK_P;
%type<nodito> RMDISK;
%type<nodito> RMDISK_L;
%type<nodito> RMDISK_P;
%type<nodito> FDISK;
%type<nodito> FDISK_L;
%type<nodito> FDISK_P;
%type<nodito> MOUNT;
%type<nodito> MOUNT_L;
%type<nodito> MOUNT_P;
%type<nodito> UNMOUNT;
%type<nodito> UNMOUNT_L;
%type<nodito> UNMOUNT_P;
%type<nodito> REP;
%type<nodito> REP_L;
%type<nodito> REP_P;
%type<nodito> EXEC;
%type<nodito> EXEC_L;
%type<nodito> EXEC_P;

//Segunda fase
%type<nodito> MKFS;
%type<nodito> MKFS_L;
%type<nodito> MKFS_P;

//Primera fase
%type<nodito> SIZE;
%type<nodito> FIT;
%type<nodito> UNIT;
%type<nodito> PATH;
%type<nodito> TYPE;
%type<nodito> DELETE;
%type<nodito> NAME;
%type<nodito> ADD;
%type<nodito> ID;
%type<nodito> COMENTARIO;

//Segunda fase



%start INICIO
%%
/*NO TERMINALES*/
//Primera fase
INICIO : LISTA                      { root = $1; }
;

LISTA: LISTA TIPOS                  { $$ = $1; $$->add(*$2); }
     | TIPOS                        { $$ = new NodoAST("root", "root", -1, -1); $$->add(*$1); }
;

TIPOS: MKDISK                       { $$ = $1; }
     | RMDISK                       { $$ = $1; }
     | FDISK                        { $$ = $1; }
     | MOUNT                        { $$ = $1; }
     | UNMOUNT                      { $$ = $1; }
     | REP                          { $$ = $1; }
     | EXEC                         { $$ = $1; }
     | COMENTARIO                   { $$ = $1; }

     | MKFS                         { $$ = $1; }
;

MKDISK: mkdisk_tk MKDISK_L          { $$ = $2; }
;

MKDISK_L: MKDISK_L MKDISK_P         { $$ = $1; $1->add(*$2); }
        | MKDISK_P                  { $$ = new NodoAST("MKDISK", "MKDISK", @1.first_line, @1.first_column); $$->add(*$1); }
;

MKDISK_P: SIZE                      { $$ = $1; }
        | FIT                       { $$ = $1; }
        | UNIT                      { $$ = $1; }
        | PATH                      { $$ = $1; }
;

RMDISK: rmdisk_tk RMDISK_L          { $$ = $2; }
;

RMDISK_L: RMDISK_L RMDISK_P         { $$ = $1; $1->add(*$2); }
        | RMDISK_P                  { $$ = new NodoAST("RMDISK", "RMDISK", @1.first_line, @1.first_column); $$->add(*$1); }
;

RMDISK_P: PATH                      { $$ = $1; }
;

FDISK: fdisk_tk FDISK_L             { $$ = $2; }
;

FDISK_L: FDISK_L FDISK_P            { $$ = $1; $1->add(*$2); }
       | FDISK_P                    { $$ = new NodoAST("FDISK", "FDISK", @1.first_line, @1.first_column); $$->add(*$1); }
;

FDISK_P: SIZE                       { $$ = $1; }
       | UNIT                       { $$ = $1; }
       | PATH                       { $$ = $1; }
       | TYPE                       { $$ = $1; }
       | FIT                        { $$ = $1; }
       | DELETE                     { $$ = $1; }
       | NAME                       { $$ = $1; }
       | ADD                        { $$ = $1; }
;

MOUNT: mount_tk MOUNT_L             { $$ = $2; }
;

MOUNT_L: MOUNT_L MOUNT_P            { $$ = $1; $1->add(*$2); }
       | MOUNT_P                    { $$ = new NodoAST("MOUNT", "MOUNT", @1.first_line, @1.first_column); $$->add(*$1); }
;

MOUNT_P: PATH                       { $$ = $1; }
       | NAME                       { $$ = $1; }
;

UNMOUNT: unmount_tk UNMOUNT_L       { $$ = $2; }
;

UNMOUNT_L: UNMOUNT_L UNMOUNT_P      { $$ = $1; $1->add(*$2); }
         | UNMOUNT_P                { $$ = new NodoAST("UNMOUNT", "UNMOUNT", @1.first_line, @1.first_column); $$->add(*$1); }
;

UNMOUNT_P: ID                       { $$ = $1; }
;

REP: rep_tk REP_L                   { $$ = $2; }
;

REP_L: REP_L REP_P                  { $$ = $1; $1->add(*$2); }
     | REP_P                        { $$ = new NodoAST("REP", "REP", @1.first_line, @1.first_column); $$->add(*$1); }
;

REP_P: PATH                         { $$ = $1; }
     | NAME                         { $$ = $1; }
     | ID                           { $$ = $1; }
;

EXEC: exec_tk EXEC_L                { $$ = $2; }
;

EXEC_L: EXEC_L EXEC_P               { $$ = $1; $1->add(*$2); }
      | EXEC_P                      { $$ = new NodoAST("EXEC", "EXEC", @1.first_line, @1.first_column); $$->add(*$1); }
;

EXEC_P: PATH                        { $$ = $1; }
;

//Segunda fase
MKFS: mkfs_tk MKFS_L                { $$ = $2; }
;

MKFS_L: MKFS_L MKFS_P               { $$ = $1; $1->add(*$2); }
      | MKFS_P                      { $$ = new NodoAST("MKFS", "MKFS", @1.first_line, @1.first_column); $$->add(*$1); }
;

MKFS_P: ID                          { $$ = $1; }
      | TYPE                        { $$ = $1; }
;


/*TERMINALES*/
//Primera fase
SIZE: size_tk igual_tk num_tk       { $$ = new NodoAST("SIZE", $3, @1.first_line, @1.first_column); }
;

FIT: fit_tk igual_tk id_tk          { $$ = new NodoAST("FIT", $3, @1.first_line, @1.first_column); }
;

UNIT: unit_tk igual_tk id_tk        { $$ = new NodoAST("UNIT", $3, @1.first_line, @1.first_column); }
;

PATH: path_tk igual_tk rut_tk       { $$ = new NodoAST("PATH", $3, @1.first_line, @1.first_column); }
;

TYPE: type_tk igual_tk id_tk        { $$ = new NodoAST("TYPE", $3, @1.first_line, @1.first_column); }
;

DELETE: delete_tk igual_tk id_tk    { $$ = new NodoAST("DELETE", $3, @1.first_line, @1.first_column); }
;

NAME: name_tk igual_tk id_tk        { $$ = new NodoAST("NAME", $3, @1.first_line, @1.first_column); }
    | name_tk igual_tk cad_tk       { $$ = new NodoAST("NAME", $3, @1.first_line, @1.first_column); }
;

ADD: add_tk igual_tk num_tk         { $$ = new NodoAST("ADD", $3, @1.first_line, @1.first_column); }
   | add_tk igual_tk nun_tk         { $$ = new NodoAST("ADD", $3, @1.first_line, @1.first_column); }
;

ID: idk_tk igual_tk id_tk           { $$ = new NodoAST("ID", $3, @1.first_line, @1.first_column); }
;

COMENTARIO: comentario_tk           { $$ = new NodoAST("COMENTARIO", $1, @1.first_line, @1.first_column); }
;

//Segunda fase


%%
