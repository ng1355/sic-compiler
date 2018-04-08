#include <stack>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <regex>

struct token{
    token(const std::string& type, const std::string& name, const int line,
          const std::string& ret = "", const std::string& param = "",
          bool defined = false); 

    std::string type;
    std::string name;
    int line; 
    /* below members are just for functions */ 
    std::string ret;
    std::string param;
    bool defined;  /* prevents function redefinition */ 
};


class token_table{
public:
    // type of following variables/ret type for function. useful for 
    // initialization lists (eg. int a, b, c...) 
    void decl_type(const char *type);
    // add either float or int
    int addvar(const std::string name, const int line_no);
    // used for function DECLERATIONS 
    int addfunc(const std::string name, const int line_no, const std::string ret, const std::string param);
    // used for function DEFINITIONS 
    int definefunc(const std::string name, const int line_no); 
    // check if we can call a function by checking if its defined 
    bool call_func(const std::string name);
    // check if a variable has been initialized, first in local,
    // then in global space 
    bool is_var_init(const std::string name);
    // if local.empty() is true, we are in global space  
    void operator ++ (); // enter scope 
    void operator -- (); // exit scope  

private:
	//scope, token list
    std::stack<std::map<std::string, token> > local;
	//name, token
    std::map<std::string, token> global;
    std::string current_type; 
};
