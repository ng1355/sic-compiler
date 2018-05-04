// sic compiler for CS-UY 3943 by Nikita Georgiou and Jack Martinez

#pragma once

#include <unordered_map>
#include <utility> 
#include <string>
#include <iostream>

const int VAR_REDEF = 0;
const int FUNC_REDECL = 1;
const int BAD_CALL = 2;
const int BAD_VAR = 3;
const int USE_FUNC_AS_VAR = 4;
const int USE_VAR_AS_FUNC = 5;

const int VAR_DECL = 0;
const int LOCAL_VAR_USE = 1;
const int GLOBAL_VAR_USE = 2;
const int FUNC_CALL = 3;
const int FUNC_DEF = 4;
const int FUNC_DECL = 5;


struct token{
    token(); 
    token(  const std::string& type, 
            const std::string& name, 
            const int line,
            const std::string& label, 
            const bool isFunc = false, 
            const std::string& param = "",
            const bool defined = false
         );

    std::string toString() const; 

    std::string type;
    std::string name;
    int line; 
    std::string label; 

    /* below members are just for functions */ 
    bool isFunc; 
    std::string param;
    bool defined;  /* prevents function redefinition */ 
};


class symbol_table{
public:
	//return the type of a variable/function
	std::string getType(const std::string& name);
	//return the line a variable/function was declared
	int getLine(const std::string& name);
	//return the parameter of a function
	std::string getparam(const std::string& name);
    // type of following variables/ret type for function. useful for 
    // initialization lists (eg. int a, b, c...)
    void decl_type(const std::string& type);

    // add either float or int
    bool addvar(const std::string& name, const std::string& label);
    
    // check if a variable has been initialized, first in local,
    // then in global space 
    bool usevar(const std::string& name);

    // used for function DECLERATIONS 
    bool addfunc(const std::string& name, 
                 const std::string& param, 
                 const std::string& label,
                 const bool defined = false);

    // used for function DEFINITIONS 
    bool definefunc(const std::string& name, 
                    const std::string& param,
                    const std::string& label);
                    
    // check if we can call a function by checking if its defined 
    // returns false if called function was not a function
    bool callfunc(const std::string& name) const;

    void enterScope();
    void exitScope();

    void printError(const int type, const token& tok = token(),
                    const std::string& name = "") const;
    void printStatus(const int type, const token& tok) const;

    // increment line number
    void operator ++ ();
    int  getlineno() const;
    // non-modifyngly retrieve token if it exists
    const token *operator [] (const std::string& name) const;

private:
    std::unordered_map<std::string, token> local;
    std::unordered_map<std::string, token> global;
    std::string current_type; 
    bool inScope = false;
    int line_no  = 1;
};
