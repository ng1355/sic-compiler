#include <stack>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <regex>

struct token{
	token();
    token(const std::string& type, const std::string& name, const int line,
          bool inScope, const std::string& param, bool defined);

    std::string type;
    std::string name;
    int line; 
	bool inScope;
    /* below members are just for functions */ 
    std::string param;
    bool defined;  /* prevents function redefinition */ 
};


class symbol_table{
public:
    // type of following variables/ret type for function. useful for 
    // initialization lists (eg. int a, b, c...) 
    void decl_type(const char *type);
    // add either float or int
    void addvar(const std::string name, const int line_no);
    // used for function DECLERATIONS 
    void addfunc(const std::string name, const std::string param, const int line_no, const bool defined);
    // used for function DEFINITIONS 
    void definefunc(const std::string name, const std::string param, const int line_no); 
    // check if we can call a function by checking if its defined 
    bool call_func(const std::string name, const int line_no);
    // check if a variable has been initialized, first in local,
    // then in global space 
    bool is_var_init(const std::string name, const int line_no);

private:
	//scope, token
    std::map<std::string, token> local;
	//name, token
    std::map<std::string, token> global;
    std::string current_type; 
};
