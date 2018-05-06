%{
    // sic compiler for CS-UY 3943 by Nikita Georgiou and Jack Martinez

    #include "assembler.hpp" 
	#include "semantic_check.hpp" 
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
    char ret_type; 
    assembler mass; 
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
    char kw[7]; /* longest KW is 6 chars (return) */ 
    char op[3]; /* ops are at most 2 chars */ 
}

%destructor { free($$); } <sval>  /* sval is always strdup'd */ 

/* hold string representations of the keyword */ 
%token <kw> KW_INT "int" <kw> KW_FLOAT "float" 
%token <kw> KW_IF "if" KW_WHILE "while" KW_RETURN "return" KW_READ "read" 
       <kw> KW_WRITE "write" KW_ELSE "else" 

/* hold the actual values of the literals */
%token <sval> ID <ival> INT_LIT <fval> FLOAT_LIT <sval> STRING_LIT  

%type <kw> kind 

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

program1: program { 
        mass.bindmain();
        mass.end();
        printf("EOF\nCode:\n");
        mass.printcode(); 
        mass.outputcode();
}

program: %empty 
| program function-def
| program decl         
| program function-decl
;

decl: kind var-list SEMICOLON
;

kind: "int" { mass.decl_type($1); }
| "float" { mass.decl_type($1); }
;

var-list: ID var-list-opt { mass.addvar($1); } 
;

var-list-opt: %empty
| var-list-opt COMMA ID { mass.addvar($3); }
;

function-decl: kind ID LPAR kind RPAR SEMICOLON 
{ 
    mass.addfunc($2, $4); 
}
;

function-def: kind ID LPAR kind ID RPAR 
{
    mass.enterScope();
    ret_type = $1[0]; 
	mass.decl_type($1);
    mass.definefunc($2, $4);
    mass.decl_type($4);
    mass.addvar($5);
    mass.funcprologue($2, $5); 
	vlist.clear();
	ilist.clear();
	flist.clear();
}
body { mass.exitScope(); } 
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
| "return" expr SEMICOLON { return_check(); mass.ret(); }
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
		boolean_check(vlist,ilist,flist);
		vlist.clear();
		ilist.clear();
		flist.clear();	
	}
;

function-call: ID LPAR expr RPAR 
	{ 
		mass.callfunc($1); 
		function_check($1,vlist.back());
		vlist.pop_back();
		vlist.push_back($1);
	} 
;

/* (-) factor [* or /] (-) factor */ 
term: addop factor term-mulop 
    { if(current_factor == 's') mass.usevar($<sval>1); }
| factor term-mulop 
    { if(current_factor == 's') mass.usevar($<sval>1); }
;

term-mulop: %empty 
| term-mulop mulop addop factor 
    { if(current_factor == 's') mass.usevar($<sval>4); }
| term-mulop mulop factor 
    { if(current_factor == 's') mass.usevar($<sval>3); } 
;

/* term +/- term */
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

/* t = tn */ 
expr: ID OP_ASSIGN expr 
	{ 
		mass.usevar($1);
		for(int i = 0; i < vlist.size(); i++)
			operation_check($1,vlist[i]);
		for(int i = 0; i < ilist.size(); i++)
			operation_check($1,ilist[i]);
		for(int i = 0; i < flist.size(); i++)
			operation_check($1,flist[i]);
		vlist.clear();
		ilist.clear();
		flist.clear();
        mass.assign($1);
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
