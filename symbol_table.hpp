#include <unordered_map>
#include <string>
#include <iostream>

struct token{
	token();
    token(  const std::string& type, 
            const std::string& name, 
            const int line,
            const std::string& param = "",
            bool defined = false
         );

    std::string type;
    std::string name;
    int line; 

    /* below members are just for functions */ 
    std::string param;
    bool defined;  /* prevents function redefinition */ 
};


class symbol_table{
public:
    // type of following variables/ret type for function. useful for 
    // initialization lists (eg. int a, b, c...) 
    void decl_type(const std::string& type);

    // add either float or int
    void addvar(const std::string& name, const int line_no);

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
    void callfunc(const std::string& name, const int line_no);

    // check if a variable has been initialized, first in local,
    // then in global space 
    void is_var_init(const std::string& name, const int line_no);

    void enterScope();
    void exitScope();

private:
    std::unordered_map<std::string, token> local;
    std::unordered_map<std::string, token> global;
    std::string current_type; 
    bool inScope; 
};
