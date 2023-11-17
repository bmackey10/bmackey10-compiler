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

#ifndef YY_YY_PARSER_TOKEN_H_INCLUDED
# define YY_YY_PARSER_TOKEN_H_INCLUDED
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
    TOKEN_EOF = 0,
    TOKEN_COMMENT = 258,
    TOKEN_IDENTIFIER = 259,
    TOKEN_INTEGER = 260,
    TOKEN_FLOAT = 261,
    TOKEN_CHAR = 262,
    TOKEN_STRING = 263,
    TOKEN_COLON = 264,
    TOKEN_SEMICOLON = 265,
    TOKEN_COMMA = 266,
    TOKEN_OPEN_BRACKET = 267,
    TOKEN_CLOSE_BRACKET = 268,
    TOKEN_OPEN_BRACE = 269,
    TOKEN_CLOSE_BRACE = 270,
    TOKEN_OPEN_PARENS = 271,
    TOKEN_CLOSE_PARENS = 272,
    TOKEN_INC = 273,
    TOKEN_DEC = 274,
    TOKEN_ASSIGN = 275,
    TOKEN_EXPONENT = 276,
    TOKEN_ADD = 277,
    TOKEN_SUBTRACT = 278,
    TOKEN_MULTIPLY = 279,
    TOKEN_DIVIDE = 280,
    TOKEN_MOD = 281,
    TOKEN_EQUALITY = 282,
    TOKEN_GE = 283,
    TOKEN_LE = 284,
    TOKEN_LESS = 285,
    TOKEN_GREATER = 286,
    TOKEN_INEQUALITY = 287,
    TOKEN_NOT = 288,
    TOKEN_AND = 289,
    TOKEN_OR = 290,
    TOKEN_ERROR = 291,
    TOKEN_TYPE = 292,
    TOKEN_FUNC = 293,
    TOKEN_ARR = 294,
    TOKEN_VOID = 295,
    TOKEN_BOOL = 296,
    TOKEN_IF = 297,
    TOKEN_ELSE = 298,
    TOKEN_FOR = 299,
    TOKEN_WHILE = 300,
    TOKEN_PRINT = 301,
    TOKEN_RETURN = 302,
    TOKEN_AUTO = 303,
    TOKEN_TYPE_BOOL = 304,
    TOKEN_TYPE_STRING = 305,
    TOKEN_TYPE_CHAR = 306,
    TOKEN_TYPE_FLOAT = 307,
    TOKEN_TYPE_INT = 308
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 13 "parser/parse.bison" /* yacc.c:1909  */

        struct decl *decl;
        struct expr *expr;
        struct stmt *stmt;
        struct type *type;
        struct param_list *param_list;
        char *name;

#line 118 "parser/token.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TOKEN_H_INCLUDED  */
