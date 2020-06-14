/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    mkdisk_tk = 258,
    rmdisk_tk = 259,
    fdisk_tk = 260,
    mount_tk = 261,
    unmount_tk = 262,
    exec_tk = 263,
    rep_tk = 264,
    mkfs_tk = 265,
    login_tk = 266,
    mkgrp_tk = 267,
    rmgrp_tk = 268,
    mkusr_tk = 269,
    rmusr_tk = 270,
    chmod_tk = 271,
    mkfile_tk = 272,
    cat_tk = 273,
    rem_tk = 274,
    edit_tk = 275,
    ren_tk = 276,
    mkdir_tk = 277,
    cp_tk = 278,
    mv_tk = 279,
    find_tk = 280,
    chown_tk = 281,
    chgrp_tk = 282,
    pause_tk = 283,
    size_tk = 284,
    fit_tk = 285,
    unit_tk = 286,
    path_tk = 287,
    type_tk = 288,
    delete_tk = 289,
    name_tk = 290,
    add_tk = 291,
    idk_tk = 292,
    igual_tk = 293,
    comentario_tk = 294,
    id_tk = 295,
    rut_tk = 296,
    cad_tk = 297,
    num_tk = 298,
    nun_tk = 299,
    usr_tk = 300,
    pwd_tk = 301,
    grp_tk = 302,
    ugo_tk = 303,
    r_tk = 304,
    p_tk = 305,
    cont_tk = 306,
    file_tk = 307,
    dest_tk = 308,
    ruta_tk = 309
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 29 "parser.yy" /* yacc.c:1909  */

//se especifican los tipo de valores para los no terminales y lo terminales
char TEXT [256];
class NodoAST *nodito;

#line 115 "parser.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
