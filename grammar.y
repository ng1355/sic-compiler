%{
	#include <iostream>
	#include <cstdio>
	using namespace std;

	extern "C" int yylex();
	extern "C" int yyparse();
	extern "C" FILE *yyin;

	void yyerror(const char* c);
%}

%skeleton "lalr1.cc"

%union{
	int ival;
	float fval;
	char *sval;
	int ikey;	//int keyword
	int fkey;	//float keyword
	int skey;	//string keyword
	int ifkey;
	int ekey;
	int wkey;
	int
	
}

%token <ival> INT_LIT
%token <fval> FLOAT_LIT
%token <sval> STRING_LIT
%token int					
%token float				
%token ID
%token if
%token else
%token while
%token read
%token write
%token return
%%
	program: { function-def | decl | function-decl }
	decl: kind var-list ';'
	kind: int | float
	var-list: ID { ',' ID }
	function-decl: kind ID '(' kind ')' ';'
	function-def: kind ID '(' kind ')' body
	body: '{' { decl } { stmt } '}'
	stmt:
		expr ';' 
		| if '(' bool-expr ')' stmt {else stmt}
		| while '(' bool-expr ')' stmt
		| read var-list ';'
		| write write-expr-list ';'
		| return expr ';'
	write-expr-list: {expr | STRING_LIT} { ',' (expr | STRING LIT) }
	factor:
		ID
		| INT_LIT
		| FLOAT_LIT
		| function-call
		| '(' expr ')'
	bool-expr: expr bool-op expr
	function-call: ID '(' expr ')'
	term: '-' factor { mullop '-' factor} 	/*	Modified rule */
	expr1: term { addop term }
	mulop: '*' | '/'
	addop: '+' | '-'
	bool-op: '<' | '>' | "==" | ">=" | "<="
	expr: ID '=' expr | expr1
%%

int main(int, char**){
	FILE *fp = f.open("TEST2", "r")
	if(!fp){
		cout << "Can't open file. Exiting.\n" << endl;
		return 1
	}
	yyin = fp;
	do{
		yyparse()
	}while(!feof(yyin))
}

void yyerror(const char* c){
	cout << "Error: " << c << endl;
	exit(1)
}
