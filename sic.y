%{
	#include <iostream>
	#include <cstdio>

	int yylex();
	int yyparse();
	int yyerror(const char* c);
	extern "C" FILE *yyin;
%}

%define parse.error verbose

%union{
	int ival;
	float fval;
	char *sval;
}

%token KW_INT "int" KW_FLOAT "float" KW_IF "if" KW_WHILE "while"
       KW_RETURN "return" KW_READ "read" KW_WRITE "write" ID KW_ELSE "else" 
       <ival> INT_LIT <fval> FLOAT_LIT <sval> STRING_LIT  
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
| program function-def 
| program decl 
| program function-decl 

decl: kind var-list SEMICOLON

kind: "int" | "float"

var-list: ID var-list-opt

var-list-opt: %empty
| var-list COMMA ID 

function-decl: kind ID LPAR kind RPAR SEMICOLON

function-def: kind ID LPAR kind RPAR body

body: LBRACE body-decl body-stmt RBRACE

body-decl: %empty
| body-decl decl 

body-stmt: %empty
| body-stmt stmt

stmt: expr SEMICOLON
| "if" LPAR bool-expr RPAR stmt else-stmt
| "while" LPAR bool-expr RPAR stmt
| "read" var-list SEMICOLON
| "write" write-expr-list SEMICOLON
| "return" expr SEMICOLON

else-stmt: %empty
| "else" stmt

write-expr-list: wel-group wel-optional 

wel-optional: %empty
| wel-optional COMMA wel-group

wel-group: expr | STRING_LIT

factor: ID
| INT_LIT { std::cout << "found int: " << $1 << '\n'; } 
| FLOAT_LIT 
| function-call
| LPAR expr RPAR

bool-expr: expr bool-op expr

function-call: ID LPAR expr RPAR

term: '-' factor term-mulop  	/*	Modified rule */

term-mulop: %empty 
| term-mulop mulop '-' factor

expr1: term expr1-addop

expr1-addop: %empty
| expr1-addop addop term

mulop: OP_MULT | OP_DIV

addop: OP_PLUS | OP_MINUS

bool-op: OP_LT | OP_GT | OP_EQ | OP_GE | OP_LE

expr: ID OP_ASSIGN expr | expr1

%%

int main(int argc, char** argv){

	FILE *fp = fopen(argv[1], "r");
	
    if(!fp){
        std::cout << "Can't open file. Exiting\n"; 
		return 1;
	}
    
    yyin = fp;
	do{
		std::cout << "Found tok: " << yyparse() << '\n';
	} while(!feof(yyin));
}

int yyerror(const char* c){
	std::cout << "|Error: " << c << "|\n";
	exit(1);
    return 1;
}
