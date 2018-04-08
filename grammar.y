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
	int int_val;
	float float_val;
	char *string_val;
	int int_key;	
	int float_key;
	int id_key;
	int string_key;
	int if_key;
	int else_key;
	int while_key;
	int read_key;
	int write_key;
	int return_key;
	
}

%token <int_val> 	INT_LIT
%token <float_val> 	FLOAT_LIT
%token <string_val> STRING_LIT
%token <int_key> 	int					
%token <float_key> 	float				
%token <id_key>		ID
%token <if_key> 	if
%token <else_key>	else
%token <while_key>	while
%token <read_key>	read
%token <write_key>	write
%token <return_key>	return
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
