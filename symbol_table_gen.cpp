#include "symbol_table_gen.hpp" 

//--------------------------
//  token struct definitions
//--------------------------

token::token(const std::string& type, const std::string& name, const int line,
             bool inScope = false, const std::string& param = "", bool defined = false):   
		     type(type), name(name), line(line), inScope(inScope), param(param),
             defined(defined) {}
token::token():type(""),name(""),line(0),inScope(false),param(""),defined(false){}
//-------------------------------
//  token_table class definitions
//-------------------------------

// set the current type
void symbol_table::decl_type(const char *type){
    current_type = type;
}

// add a variable to the token table 
void symbol_table::addvar(const std::string name, const int line_no){
    // if local is empty, we're in global space.
	bool in_local_space = !local.empty();
	token variable = token(current_type,name,line_no);
	if(in_local_space){
		variable.inScope = true;
		local[name] = variable;
	}else{
		global[name] = variable;
	}
}

// add a function declaration 
// returns 0 if successful, -1 if that function was already declared 
void symbol_table::addfunc(const std::string name, const std::string param,
						   const int line_no, const bool defined){
	if(global.count(name)) return;
	global[name] = token(current_type,name,line_no,false,param,defined);
	if(!defined) std::cout << "Function "  << current_type << ' ' << name << "(" << 
                           param << ") declared in line " << line_no << "\n"; 
    return;
}
void symbol_table::definefunc(const std::string name, const std::string param, const int line_no){
	if(global.count(name)){ 
		if(!global[name].defined)
			global[name].defined = true;
		else
			std::cout << "Error: function redefinition on line " << line_no << "\n";
	}else{
		global[name] = token(current_type,name,line_no,false,param,true);
	}
}

// check if a function is defined 
bool symbol_table::call_func(const std::string name, const int line_no){
	if(global.count(name) && global[name].defined){
		std::cout << "Function " << name << " defined in line " << 
			      global[name].line << " and used in line " << line_no << "\n";
	}else{
		std::cout << "Error:Unknown function " << name << " referenced on line " << line_no << "\n";
	}
}
//check if a variable has been initialized
bool symbol_table::is_var_init(const std::string name, const int line_no){
	int line_dec;
	if(local.count(name)){
		std::cout << "Local variable ";
		line_dec = local[name].line;
	}else if(global.count(name)){
		std::cout << "Global variable ";
		line_dec = global[name].line;
	}
	std::cout << global[name].name << " declared in line " << line_dec <<
			  " and used in line " << line_no << "\n";
}
int main(){
	std::cout << "Garbage :)\n";
}
