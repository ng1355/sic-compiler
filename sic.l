%{
// sic compiler for CS-UY 3943 by Nikita Georgiou and Jack Martinez

#define register  /* Removes deprecated keyword from flex files */ 
#include "sic.tab.h" 
#include "symbol_table.hpp"


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream> 

char op_name[20];
char *skip;
int mylength;
extern symbol_table table;

%}

%x comment
%option noyywrap
%option yylineno

%%

 /* Code to deal with comments borrowed from GNU flex manual. */
"/*"		BEGIN(comment);
<comment>[^*\n]*	   /* eat anything that's not a '*' */
<comment>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<comment>\n		   ++table;
<comment>"*"+"/"	   BEGIN(INITIAL);

 /* tokens */
"int"			{  yylval.ival = 0; return KW_INT; } 
"float"			{  yylval.ival = 1; return KW_FLOAT; } 
"if"			return(KW_IF);
"else"			return(KW_ELSE);
"while"			return(KW_WHILE);
"return"		return(KW_RETURN);
"read"			return(KW_READ);
"write"			return(KW_WRITE);
"+"                     return(OP_PLUS);
"-"                     return(OP_MINUS);
"*"                     return(OP_MULT);
"/"                     return(OP_DIV);
"="                     return(OP_ASSIGN);
"=="                    return(OP_EQ); /* equal */
"<"                     return(OP_LT); /* less than */
">"                     return(OP_GT); /* less than or equal */
"<="                    return(OP_LE); /* greater than */
">="                    return(OP_GE); /* greater than or equal */
"("                     return(LPAR);
")"                     return(RPAR);
"{"                     return(LBRACE);
"}"                     return(RBRACE);
";"                     return(SEMICOLON);
","                     return(COMMA);

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
				      skip, table.getlineno());
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
				      skip, table.getlineno());
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
\n			{ ++table; /* not a token; add return(NL_TOKEN) for testing */ }
[\t ]+			{} /* skip white space; add return(WS_TOKEN) for testing. */ 
%%

void yyerror(const char *c){
    std::cout << c << " on line " << table.getlineno() << ", matched: " <<
    yytext << '\n';
}
