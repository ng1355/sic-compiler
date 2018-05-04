%{
// sic compiler for CS-UY 3943 by Nikita Georgiou and Jack Martinez

#define register  /* Removes deprecated keyword from flex files */ 
#include "assembler.hpp"
#include "sic.tab.h" 
#include "semantic_check.hpp"


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream> 

char op_name[20];
char *skip;
int mylength;
extern assembler mass;

%}

%x comment
%option noyywrap
%option yylineno

%%

 /* Code to deal with comments borrowed from GNU flex manual. */
"/*"		BEGIN(comment);
<comment>[^*\n]*	   /* eat anything that's not a '*' */
<comment>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<comment>\n		   ++mass;
<comment>"*"+"/"	   BEGIN(INITIAL);

 /* tokens */
"int"			{  strcpy(yylval.kw, "int"); return KW_INT; } 
"float"			{  strcpy(yylval.kw, "float"); return KW_FLOAT; } 
"if"	        {  strcpy(yylval.kw, "if"); return KW_IF; } 	
"else"          {  strcpy(yylval.kw, "else"); return KW_ELSE; } 	
"while"         {  strcpy(yylval.kw, "while"); return KW_WHILE; } 
"return"	    {  strcpy(yylval.kw, "return"); return KW_RETURN; } 
"read"	        {  strcpy(yylval.kw, "read"); return KW_READ; } 
"write"         {  strcpy(yylval.kw, "write"); return KW_WRITE; } 
"+"             {  strcpy(yylval.op, "+"); return OP_PLUS; } 
"-"             {  strcpy(yylval.op, "-"); return OP_MINUS; }
"*"             {  strcpy(yylval.op, "*"); return OP_MULT; } 
"/"             {  strcpy(yylval.op, "/"); return OP_DIV; } 
"="             {  strcpy(yylval.op, "="); return OP_ASSIGN; } 
"=="            {  strcpy(yylval.op, "=="); return OP_EQ; } 
"<"            	{  strcpy(yylval.op, "<"); return OP_LT; }
">"             {  strcpy(yylval.op, ">"); return OP_GT; } 
"<="            {  strcpy(yylval.op, "<="); return OP_LE; } 
">="            {  strcpy(yylval.op, ">="); return OP_GE; } 
"("             {  strcpy(yylval.op, "("); return LPAR; } 
")"             {  strcpy(yylval.op, ")"); return RPAR; } 
"{"             {  strcpy(yylval.op, "{"); return LBRACE; } 
"}"             {  strcpy(yylval.op, "}"); return RBRACE; } 
";"             {  strcpy(yylval.op, ";"); return SEMICOLON; } 
","             {  strcpy(yylval.op, ","); return COMMA; } 

[a-zA-Z_][a-zA-Z_0-9]*  { 
			   yylval.sval = strdup(yytext); 
			   return(ID); 
                        }

[0-9]+			{ for (mylength=yyleng, skip=yytext;
			       *skip =='0';
			       skip++, mylength--)
			    ;
			  if (mylength>6) 
			    {
			      fprintf(stderr,
				      "Suspiciously large interger literal %s in line %d. "
                      "Replacing by 0.\n",
				      skip, mass.getlineno());
			      yylval.ival=0;
			    }
			  else 
			    {
			      yylval.ival=(int)atol(skip);
			    }
			  return (INT_LIT); 
			}

[0-9]+\.[0-9]+          { 
			  yylval.fval=atof(yytext);
			  if (yyleng>20)
			    {
			      fprintf(stderr,
				      "Suspiciously long float literal %s in line %d. " 
                      "Replacing by 0.0.\n",
				      skip, mass.getlineno());
			      yylval.fval=0;
			    }
			  return (FLOAT_LIT);
			}

\"[^\"]*\"		{ /* string literal; not in original language description. */
               yytext[yyleng - 1] = '\0';
			   yylval.sval = strdup(yytext + 1);
			   return(STRING_LIT);
			}

  /* The ones below are not really tokens, but 
   * things ignored by the compiler, except for counting lines. */
\n			{ ++mass; /* not a token; add return(NL_TOKEN) for testing */ }
[\t ]+			{} /* skip white space; add return(WS_TOKEN) for testing. */ 
%%

void yyerror(const char *c){
    std::cout << c << " on line " << mass.getlineno() << ", matched: " <<
    yytext << '\n';
}
