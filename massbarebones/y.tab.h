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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ADD = 258,
    ADDF = 259,
    MUL = 260,
    MULF = 261,
    DIV = 262,
    DIVF = 263,
    SUB = 264,
    SUBF = 265,
    NEG = 266,
    NEGF = 267,
    COPY = 268,
    COPYF = 269,
    LABEL = 270,
    JUMP = 271,
    JEQ = 272,
    JEQF = 273,
    JNE = 274,
    JNEF = 275,
    JLT = 276,
    JLTF = 277,
    JLE = 278,
    JLEF = 279,
    JGE = 280,
    JGEF = 281,
    JGT = 282,
    JGTF = 283,
    CALL = 284,
    RETURN = 285,
    READ = 286,
    READF = 287,
    WRITE = 288,
    WRITEF = 289,
    WRITES = 290,
    NEWLINE = 291,
    STOP = 292,
    START = 293,
    PUSH = 294,
    PUSHF = 295,
    POP = 296,
    NL = 297,
    ADDRESS = 298,
    CONSTANT = 299,
    CONSTANTF = 300,
    INDIR = 301,
    STRING = 302,
    SYMBOL = 303
  };
#endif
/* Tokens.  */
#define ADD 258
#define ADDF 259
#define MUL 260
#define MULF 261
#define DIV 262
#define DIVF 263
#define SUB 264
#define SUBF 265
#define NEG 266
#define NEGF 267
#define COPY 268
#define COPYF 269
#define LABEL 270
#define JUMP 271
#define JEQ 272
#define JEQF 273
#define JNE 274
#define JNEF 275
#define JLT 276
#define JLTF 277
#define JLE 278
#define JLEF 279
#define JGE 280
#define JGEF 281
#define JGT 282
#define JGTF 283
#define CALL 284
#define RETURN 285
#define READ 286
#define READF 287
#define WRITE 288
#define WRITEF 289
#define WRITES 290
#define NEWLINE 291
#define STOP 292
#define START 293
#define PUSH 294
#define PUSHF 295
#define POP 296
#define NL 297
#define ADDRESS 298
#define CONSTANT 299
#define CONSTANTF 300
#define INDIR 301
#define STRING 302
#define SYMBOL 303

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
