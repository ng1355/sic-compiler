#include "token_table.hpp" 

//--------------------------
//  token struct definitions
//--------------------------

token::token(const std::string& type, const std::string& name, const int line,
             const int param, const bool defined):   
		     type(type), name(name), line(line), param(param),
             defined(defined) {}

//-------------------------------
//  token_table class definitions
//-------------------------------

// set the current type
void token_table::decl_type(const char *type){
//    std::cout <<  "declared type: " << type << '\n';
    current_type = type;
}

// add a variable to the token table 
// returns 0 on sucess, -1 on failure
void token_table::addvar(const std::string name, const int line_no){
    // if local is empty, we're in global space.
	struct token var = token(current_type, name, line_no);
    if(local.empty()){
    	if(global.count(name)) return; 
		global.insert(std::pair<std::string, struct token>(name, var));
		std::cout << "Global " << current_type << " variable ";
    } else {
        if(local.top().count(name)) return;
		local.top().insert(std::pair<std::string, struct token>(name, var));
		std::cout << "Local " << current_type << " variable ";
    }
	std::cout << name << " declared in line " << line_no << "\n";
	return;
}

// add a function declaration 
// returns 0 if successful, -1 if that function was already declared 
void token_table::addfunc(const std::string name, const int param, const int line_no, const bool defined){
    if(global.count(name)) return;
	struct token func = token(current_type, name, line_no, param, defined);
	global.insert(std::pair<std::string, struct token>(name, func));
	if(!defined) std::cout << "Function "  << current_type << ' ' << name << "(" << 
        ((param) ? "float" : "int") << ") declared in line " << line_no << "\n"; 
    return;
}

// add a function definition
// returns 0 if successful, -1 if its already been declared
void token_table::definefunc(const std::string name, const int param, const int line_no){
    if(global.count(name)){
		std::map<std::string, struct token>::iterator func = global.find(name);
		if(func->second.defined){
            std::cout << "Error: Function " << name << " redefinition on line " << 
                line_no << '\n';
            return;
        }
		func->second.defined = true;
		std::cout << "Function " << name << " defined in line " << line_no << "\n";
        return;
    } else {
        addfunc(name, param, line_no, true);
		std::cout << "Function " << current_type << ' ' << name << 
            '(' << ((param) ? "float" : "int") << ") defined in line " 
            << line_no << "\n";
    }
}

// check if a function is defined 
void token_table::call_func(const std::string name, const int line_no){
    auto func = global.find(name);
    if(func != global.end()){
        std::cout << "Function " << name << ((func->second.defined) ? " defined" : 
            " declared") << " in line " << func->second.line <<
            " used in line " << line_no << '\n';
    } else {
        std::cout << "Error: No such function " << name << " on line " <<
            line_no << '\n';
    }
}

//check if a variable has been initialized
//doesnt work help me GOD 
void token_table::is_var_init(const std::string name, const int line_no){
    /* 
	std::stack<std::map<std::string,token> > copy;
	while(!local.empty()){
		copy.push(local.top());
		local.pop();
		std::map<std::string, struct token>::iterator var_l = copy.top().find(name);
		if(var_l  == copy.top().end())
			continue;
        std::cout << "Local variable " << name <<
            "declared in line " << var_l->second.line << " used in line " <<
            line_no << '\n';
		return;
	}
	//fix the local stack
	while(!copy.empty()){
		local.push(copy.top());
		copy.pop();
	}
	std::map<std::string, struct token>::iterator var_g = global.find(name);
	if(var_g != global.end()){
        std::cout << "Global variabel " << name << 
            " declared in line " << var_g->second.line << " used in line " <<
            line_no << '\n';
            return;
    }
    std::cout << "Error: Variable " << name << " not previously defined.\n";
    !!!! SECOND ATTEMP !!!!!

    if(!local.empty()){
        if(local.top().count(name)){
            std::cout << "Local variable " << name << " declared in line " <<
                local.top()[name].line << " used in line " << line_no << '\n';
            return;
        }
    } else {
        if(global.count(name)){
            std::cout << "Global variable " << name << "declared in line " << 
                global[name].line << " used in line " << line_no << '\n';
            return;
        } else {
            std::cout << "Error: Variable referenced before assignment\n";
        }
    } */
}

void token_table::operator ++ (){
	std::map<std::string, struct token> scope;
	local.push(scope);
//    std::cout << "Entering scope " << local.size() << '\n';
}
void token_table::operator -- (){
//    std::cout << "Exiting scope " << local.size() << '\n';
	local.pop();
//    std::cout << "Returned to scope " << local.size() << '\n';
}
