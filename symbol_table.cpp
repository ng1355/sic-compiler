#include "symbol_table.hpp" 

//--------------------------
//  token struct definitions
//--------------------------

    
token::token() = default; 
token::token(   const std::string& type, 
                const std::string& name, 
                const int line,
                const std::string& param,
                bool defined
            ):   
		     type(type), 
             name(name), 
             line(line), 
             param(param),
             defined(defined) {}

std::string token::toString() const {
    return (param.empty()) ? 
            type + ' ' + name
            : type + ' ' + name + " (" + param + ") "; 
}
    
//-------------------------------
//  token_table class definitions
//-------------------------------

// set the current type
void symbol_table::decl_type(const std::string& type){
    current_type = type;
}

// add a variable to the token table 
void symbol_table::addvar(const std::string& name, const int line_no){
    if(inScope){
        auto [ it, added ] = 
            local.emplace(name, token{current_type, name, line_no});
        if(added) printStatus(VAR_DECL, line_no, it->second);
        else printError(VAR_REDEF, line_no, it->second);
    } else {
        auto [ it, added ] = 
            global.emplace(name, token{current_type, name, line_no});
        if(added) printStatus(VAR_DECL, line_no, it->second);
        else printError(VAR_REDEF, line_no, it->second);
    }
}


//check if a variable has been initialized
void symbol_table::usevar(const std::string& name, const int line_no){
    puts("entering uservar");
    auto it = local.find(name);
    if(it != local.end()) printStatus(LOCAL_VAR_USE, line_no, it->second);
    else if(it = global.find(name); it != global.end()) 
        printStatus(GLOBAL_VAR_USE, line_no, it->second);
    else printError(BAD_VAR, line_no);
    puts("exiting uservar");
}

// add a function declaration 
void symbol_table::addfunc( const std::string& name, 
                            const std::string& param,
						    const int line_no, 
                            const bool defined)
{
    auto [ it, added ] = 
        global.emplace(name, token{current_type, name, line_no, param, defined});
    if(!added) printError(FUNC_REDECL, line_no, it->second);
    else if(defined) printStatus(FUNC_DEF, line_no, it->second);
    else printStatus(FUNC_DECL, line_no, it->second);
}

void symbol_table::definefunc(  const std::string& name, 
                                const std::string& param, 
                                const int line_no)
{
    addfunc(name, param, line_no, true);
}

// check if a function is defined 
void symbol_table::callfunc(const std::string& name, const int line_no) const {
    auto it = global.find(name);
    if(it != global.end()) printStatus(FUNC_CALL, line_no, it->second);
	else printError(BAD_CALL, line_no); 
}

void symbol_table::enterScope(){
    inScope = true; 
}

void symbol_table::exitScope(){
    inScope = false;
    local.clear();
}

void symbol_table::printError(const int type, const int line_no,
        const token& tok, const std::string& name) const {

    switch(type){
        case VAR_REDEF:
            std::cerr  << "Error: " << (inScope ? " local" : " global")
                       << " variable " << tok.toString()
                       << " declared on line " << line_no
                       << " already declared on line " << tok.line << '\n';
            break;
        case FUNC_REDECL:
            std::cerr  << "Error: function " << tok.toString()
                       << (tok.defined ? " defined" : " declared")
                       << " on line " << line_no
                       << " already declared on line " << tok.line << '\n';
            break;
        case BAD_CALL:
        case BAD_VAR:
            std::cerr  << "Error: unknown "
                       << ((type == BAD_VAR) ? "variable " : "function ")
                       << name << " referenced on line " << line_no << '\n';
    }
}

void symbol_table::printStatus(const int type, const int line_no,
        const token& tok) const { 

    switch(type){
        case VAR_DECL:
            std::cout << (inScope ? "Local " : "Global ") 
                      << tok.toString() << " variable declared  on line "
                      << line_no << '\n';
            break;
        case LOCAL_VAR_USE:
        case GLOBAL_VAR_USE:
            std::cout << ((type == GLOBAL_VAR_USE) ? "Global" : "Local")
                      << " variable " << tok.name << " declared in line "
                      << tok.line << " used in line " << line_no << '\n';
            break;
        case FUNC_CALL:
            std::cout << "Function " << tok.name
                      << (tok.defined ? " defined" : " declared") 
                      << " in line " << tok.line << " used in line "
                      << line_no << '\n';
            break;
        case FUNC_DEF:
            std::cout << "Function " << tok.name << " defined in line "
                      << line_no << '\n';
            break;
        case FUNC_DECL:
            std::cout << "Function " << tok.toString()
                      << " declared in line " << line_no << '\n';
    }
    puts("exiting printstats");
}
