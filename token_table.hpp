#include <stack>
#include <unordered_map> 

struct token{
    token(const std::string& type, const std::string& name, const int line,
            const std::string& ret = "", const std::string& param = "",
            const bool defined = false); 

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
    /* type of following variables/ret type for function. useful for 
     * initialization lists (eg. int a, b, c...) */ 
    void decl_type(const char *type);
    /* add either float or int. */ 
    int addvar(const char *name, const int line_no);
    /* used for function DECLERATIONS */ 
    int addfunc(const char *name, const char *param, const int line_no);
    /* used for function DEFINITIONS */ 
    int definefunc(const char *name, const char *param, const int line_no); 

    /* check if we can call a function by checking if its defined */ 
    int call_function(const char *name);
    /* check if a variable has been initialized, first in local,
     * then in global space */ 
    int use_var(const char *name);

    /* if local.empty() is true, we are in global space */ 
    void operator ++ (); /* enter scope */ 
    void operator -- (); /* exit scope */ 

private:
    std::stack<std::unordered_map<token> > local;
    std::unordered_map<token> global;
    std::string current_type; 
};
