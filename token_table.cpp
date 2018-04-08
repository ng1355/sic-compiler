#include "token_table.hpp" 

//--------------------------
//  token struct definitions
//--------------------------

token::token(const std::string& type, const std::string& name, const int line,
             const std::string& ret, const std::string& param,
             bool defined):   
		     type(type), name(name), line(line), ret(ret), param(param), 
             defined(defined) 
	         {}
//-------------------------------
//  token_table class definitions
//-------------------------------

// set the current type
void token_table::decl_type(const char *type){
    current_type = type;
}

// add a variable to the token table 
// returns 0 on sucess, -1 on failure
int token_table::addvar(const std::string name, const int line_no){
    // if local is empty, we're in global space.
	struct token var = token(current_type, name, line_no);
    if(local.empty()){
    	if(global.count(name)) return -1;
		global.insert(std::pair<std::string, struct token>(name, var));
		std::cout << "Global variable ";
    } else {
        if(local.top().count(name)) return -1;
		local.top().insert(std::pair<std::string, struct token>(name, var));
		std::cout << "Local variable ";
    }
	std::cout << name << " declared in line " << line_no << "\n";
	return 0;
}

// add a function declaration 
// returns 0 if successful, -1 if that function was already declared 
int token_table::addfunc(const std::string name, const int line_no, const std::string ret,
						 const std::string param){
    if(global.count(name)) return -1;
	struct token func = token(current_type, name, line_no, ret, param);
	global.insert(std::pair<std::string, struct token>(name, func));
	std::cout << "Function " << name << "(" << param << ") declared in line " << line_no << "\n"; 
    return 0;
}

// add a function definition
// returns 0 if successful, -1 if its already been declared
int token_table::definefunc(const std::string name, const int line_no){
    if(global.count(name)){
		std::map<std::string, struct token>::iterator func = global.find(name);
		if(func->second.defined) return -1;
		func->second.defined = true;
		std::cout << "Function " << name << " defined in line " << line_no << "\n";
        return 0;
    }
}
// check if a function is defined 
bool token_table::call_func(const std::string name){
	std::map<std::string, struct token>::iterator func = global.find(name);
	return func->second.defined;
}
//check if a variable has been initialized
bool token_table::is_var_init(const std::string name){
	std::stack<std::map<std::string,token>> copy;
	while(!local.empty()){
		copy.push(local.top());
		local.pop();
		std::map<std::string, struct token>::iterator var_l = copy.top().find(name);
		if(var_l  == copy.top().end())
			continue;
		return true;
	}
	//fix the local stack
	while(!copy.empty()){
		local.push(copy.top());
		copy.pop();
	}
	std::map<std::string, struct token>::iterator var_g = global.find(name);
	if(var_g != global.end()) return true;
	return false;
}
void token_table::operator ++ (){
	std::map<std::string, struct token> scope;
	local.push(scope);
}
void token_table::operator -- (){
	local.pop();
}

int main(){
	std::cout << "a";
}
