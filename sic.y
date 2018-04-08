%{
	#include <iostream>
	#include <cstdio>

	int yylex();
	int yyparse();
    int yyerror(const char* c);
	extern "C" FILE *yyin;
%}

%define parse.error verbose
%locations

%union{
	int ival;
	float fval;
	char *sval;
}

%token KW_INT "int" KW_FLOAT "float" KW_IF "if" KW_WHILE "while"
       KW_RETURN "return" KW_READ "read" KW_WRITE "write" KW_ELSE "else" 
       <sval> ID <ival> INT_LIT <fval> FLOAT_LIT <sval> STRING_LIT  
%left OP_PLUS '+' OP_MINUS '-' 
%left OP_MULT '*' OP_DIV '/' 
%right OP_ASSIGN '='
%left OP_EQ "==" OP_LT '<' OP_LE "<=" OP_GT '>' OP_GE ">=" 
%left LPAR '(' RPAR ')'
%left LBRACE '{' RBRACE '}'
%token SEMICOLON ';'
%left COMMA ','
       
/* temporary ugly fix to silence s/r conflict from dangling-else. 
 * Bison makes the right choice anyway */ 
%%

program: %empty 
| program function-def { puts("program function-def"); }
| program decl         { puts("program decl"); }
| program function-decl { puts("program function-decl"); } 
;

decl: kind var-list SEMICOLON { puts ("kind var-list ;"); }
;

kind: "int" | "float" { puts("int | float"); }
;

var-list: ID var-list-opt { std::cout << "varlist: " << $1 << '\n'; }
;

var-list-opt: %empty
| var-list-opt COMMA ID  { std::cout << "var-list-opt COMMA " << $3 << '\n'; }
;

function-decl: kind ID LPAR kind RPAR SEMICOLON {std::cout << "Function decl name: " << $2 << '\n'; } 
;

function-def: kind ID LPAR kind ID RPAR body { std::cout << "kind" << $2 << \
                "LPAR kind " << $5 << "RPAR body\n"; }
;

body: LBRACE body-decl body-stmt RBRACE { puts("{ body-decl body-stmt }"); }
;

body-decl: %empty
| body-decl decl  
;

body-stmt: %empty
| body-stmt stmt
;

stmt: expr SEMICOLON
| "if" LPAR bool-expr RPAR stmt "else" stmt{ puts("if"); }
| "if" LPAR bool-expr RPAR stmt { puts("if"); }
/* unofficial while-stmt that accepts bodies in addition to statement */ 
| "while" LPAR bool-expr RPAR while-stmt { puts("while"); } 
| "read" var-list SEMICOLON { puts("read"); } 
| "write" write-expr-list SEMICOLON { puts("write"); }
| "return" expr SEMICOLON { puts("return"); }
;

while-stmt: body
| stmt
;

write-expr-list: wel-group wel-optional  { puts("write-expr-list"); }
;

wel-optional: %empty
| wel-optional COMMA wel-group { puts("wel-optional"); }
;

wel-group: expr { puts("expr"); }
| STRING_LIT { std::cout << "STRING_LIT value: " << $1 << '\n'; }
/* unofficial var-list for comma delimited IDs */ 
;

factor: ID { std::cout << "found ID: " << $1 << '\n'; }
| INT_LIT { std::cout << "found int: " << $1 << '\n'; } 
| FLOAT_LIT { std::cout << "found float: " << $1 << '\n'; }
| function-call { std::cout << "function-call in factor\n"; }
| LPAR expr RPAR { puts("LPAR expr RPAR"); }
;

bool-expr: expr bool-op expr { puts("bool-expr"); }
;

function-call: ID LPAR expr RPAR { puts("function-call"); }
;

term:  addop factor term-mulop 
| factor term-mulop
;

term-mulop: %empty 
| term-mulop mulop addop factor { puts("term-mulop"); }
| term-mulop mulop factor
;

expr1: term expr1-addop
;

expr1-addop: %empty
| expr1-addop addop term
;

mulop: OP_MULT | OP_DIV
;

addop: OP_PLUS | OP_MINUS
;

bool-op: OP_LT | OP_GT | OP_EQ | OP_GE | OP_LE
;

expr: ID OP_ASSIGN expr { std::cout << $1 << " = expr\n"; }
| expr1
;
%%

int main(int argc, char** argv){

	FILE *fp = fopen(argv[1], "r");
	
    if(!fp){
        std::cout << "Can't open file. Exiting\n"; 
		return 1;
	}
    
    yyin = fp;
	do{
        yyparse();
	} while(!feof(yyin));
}
/*
int yyerror(const char* c){
	std::cout << c << " on line " << yylineno << '\n';
    return 1;
}
*/
