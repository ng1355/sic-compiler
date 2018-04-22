#include <unordered_map>
#include <utility> 
#include <string>
#include <iostream>

const int VAR_REDEF = 0;
const int FUNC_REDECL = 1;
const int BAD_CALL = 2;
const int BAD_VAR = 3;

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
            const std::string& param = "",
            bool defined = false
         );

    std::string toString() const; 

    std::string type;
    std::string name;
    int line; 

    /* below members are just for functions */ 
    std::string param;
    bool defined;  /* prevents function redefinition */ 
};


class symbol_table{
public:
	//return the type of a variable/function
	const std::string getType(const std::string& name);
	//return the line a variable/function was declared
	const int getLine(const std::string& name);
	//return the parameter of a function
	const std::string getparam(const std::string& name);
    // type of following variables/ret type for function. useful for 
    // initialization lists (eg. int a, b, c...)
    void decl_type(const std::string& type);

    // add either float or int
    void addvar(const std::string& name, const int line_no);
    
    // check if a variable has been initialized, first in local,
    // then in global space 
    void usevar(const std::string& name, const int line_no);

    // used for function DECLERATIONS 
    void addfunc(const std::string& name, 
                 const std::string& param, 
                 const int line_no, 
                 const bool defined = false
                );

    // used for function DEFINITIONS 
    void definefunc(const std::string& name, 
                    const std::string& param, 
                    const int line_no); 

    // check if we can call a function by checking if its defined 
    void callfunc(const std::string& name, const int line_no) const;


    void enterScope();
    void exitScope();

    void printError(const int type, const int line_no,
            const token& tok = token(), const std::string& name = "") const;
    void printStatus(const int type, const int line_no, const token& tok)const;

private:
    std::unordered_map<std::string, token> local;
    std::unordered_map<std::string, token> global;
    std::string current_type; 
    bool inScope; 
};
