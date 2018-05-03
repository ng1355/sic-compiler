%{
    // sic compiler for CS-UY 3943 by Nikita Georgiou and Jack Martinez

    #include "symbol_table.hpp"
	#include "semantic_check.cpp" 
    #include <string> 
	#include <iostream>
	#include <cstdio>
	#include <vector>

	int yylex();
	int yyparse();
    int yyerror(const char* c);
	extern "C" FILE *yyin;

    /* Used to determine the last updated type of "factor."
     * 's' means "sval" (string - for IDs)
     * 'i' means "ival" (int - for INT_LIT) 
     * 'f' means "fval" (float - for FLOAT_LIT) */ 
    char current_factor; 
    int ret_type; 
    symbol_table table;
	std::vector<char*> vlist;
	std::vector<int> ilist;
	std::vector<float> flist;
%}

%define parse.error verbose
%locations

%union{
    int ival;
    float fval;
    char *sval;
    char keyword[7]; /* longest KW is 6 chars (return) */ 
    char op[3];      /* ops are at most 2 chars */ 
}

%destructor { free($$); } <sval>  /* sval is always strdup'd */ 

%token <ival> KW_INT "int" <ival> KW_FLOAT "float" 
%token <sval> ID <ival> INT_LIT <fval> FLOAT_LIT <sval> STRING_LIT  
%type <ival> kind
%token KW_IF "if" KW_WHILE "while" KW_RETURN "return" KW_READ "read" 
       KW_WRITE "write" KW_ELSE "else" 

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
 %expect 1
%%

program: %empty 
| program function-def
| program decl         
| program function-decl
;

decl: kind var-list SEMICOLON
;

kind: "int" { table.decl_type("int"); $$ = $1; } 
| "float" { table.decl_type("float"); $$ = $1; }
;

var-list: ID var-list-opt { table.addvar($1); } 
;

var-list-opt: %empty
| var-list-opt COMMA ID { table.addvar($3); }
;

function-decl: kind ID LPAR kind RPAR SEMICOLON 
{ 
    table.addfunc($2, ($4 ? "float" : "int")); 
}
;

function-def: kind ID LPAR kind ID RPAR 
{
    table.enterScope();
    ret_type = $1; 
	table.decl_type(($1 ? "float" : "int"));
    table.definefunc($2, ($4 ? "float" : "int"));
    table.decl_type(($4 ? "float" : "int"));
    table.addvar($5);
	vlist.clear();
	ilist.clear();
	flist.clear();
}
body { table.exitScope(); } 
;

body: LBRACE body-decl body-stmt RBRACE
;

body-decl: %empty
| body-decl decl 
;

body-stmt: %empty
| body-stmt stmt
;

stmt: expr SEMICOLON
| "if" LPAR bool-expr RPAR stmt "else" stmt 
| "if" LPAR bool-expr RPAR stmt 
| "while" LPAR bool-expr RPAR stmt
| "read" var-list SEMICOLON
| "write" write-expr-list SEMICOLON
| "return" expr SEMICOLON { return_check(table); }
| LBRACE opt-stmt RBRACE
;

opt-stmt: %empty
| opt-stmt stmt
;

write-expr-list: wel-group wel-optional
;

wel-optional: %empty
| wel-optional COMMA wel-group
;

wel-group: expr
| STRING_LIT
;

factor: ID  
	{ 
		$<sval>$ = $1; 
		current_factor = 's'; 
		vlist.push_back($1);
	}
| INT_LIT   
	{ 
		$<ival>$ = $1; 
		current_factor = 'i'; 
		ilist.push_back($1);
	}
| FLOAT_LIT 
	{ 
		$<fval>$ = $1; 
		current_factor = 'f'; 
		flist.push_back($1);
	}
| function-call
| LPAR expr RPAR
;

bool-expr: expr bool-op expr
	{
		boolean_check(vlist,ilist,flist,table);
		vlist.clear();
		ilist.clear();
		flist.clear();	
	}
;

function-call: ID LPAR expr RPAR 
	{ 
		table.callfunc($1); 
		function_check($1,vlist.back(),table);
		vlist.pop_back();
		vlist.push_back($1);
	} 
;

term: addop factor term-mulop 
    { if(current_factor == 's') table.usevar($<sval>1); }
| factor term-mulop 
    { if(current_factor == 's') table.usevar($<sval>1); }
;

term-mulop: %empty 
| term-mulop mulop addop factor 
    { if(current_factor == 's') table.usevar($<sval>4); }
| term-mulop mulop factor 
    { if(current_factor == 's') table.usevar($<sval>3); } 
;

expr1: term expr1-addop
;

expr1-addop: %empty
| expr1-addop addop term
;

mulop: OP_MULT 
| OP_DIV
;

addop: OP_PLUS 
| OP_MINUS
;

bool-op: OP_LT
| OP_GT       
| OP_EQ      
| OP_GE     
| OP_LE    
;

expr: ID OP_ASSIGN expr 
	{ 
		table.usevar($1);
		for(int i = 0; i < vlist.size(); i++)
			operation_check($1,vlist[i],table);
		for(int i = 0; i < ilist.size(); i++)
			operation_check($1,ilist[i],table);
		for(int i = 0; i < flist.size(); i++)
			operation_check($1,flist[i],table);
		vlist.clear();
		ilist.clear();
		flist.clear();
	}
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
Sample input: 
int y;
int test(int); 
int inc(int x)
{
return x+1;
}
float main (float z)
{ int x;
x=7+y;
write inc(x);
}

output:
Global int variable y declared in line 1
Function int test(int) declared in line 2
Function int inc(int) defined in line 6
Local int variable x declared in line 8
Function inc defined in line 6 used in line 10
Function int main(float) defined in line 11/
*/
