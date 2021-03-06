// #sic compiler for CS-UY 3943 by Nikita Georgiou and Jack Martinez

#include "symbol_table.hpp" 

//--------------------------
//  token struct definitions
//--------------------------

    
token::token() = default; 
token::token(   const std::string& type, 
                const std::string& name, 
                const int line,
                const std::string& label, 
                const bool isFunc,
                const std::string& param,
                const bool defined
            ):   
		     type(type), 
             name(name), 
             line(line), 
             label(label), 
             isFunc(isFunc),
             param(param),
             defined(defined) {}

std::string token::toString() const {
    return (!isFunc) ? 
            type + ' ' + name
            : type + ' ' + name + " (" + param + ") "; 
}

//-------------------------------
//  oken_table class definitions
//-------------------------------
//return the type of a variable/function
std::string symbol_table::getType(const std::string& name){
	if(local.count(name)) return local[name].type;
	if(global.count(name)) return global[name].type;
    return "Fail";
}
//get the line number where a variable/function was declared
int symbol_table::getLine(const std::string& name){
	if(local.count(name)) return local[name].line;
	if(global.count(name)) return global[name].line;
    return -1;
}
std::string symbol_table::getparam(const std::string& name){
	if(global.count(name)) return global[name].param;
    return "Fail";
}

// set the current type
void symbol_table::decl_type(const std::string& type){
    current_type = type;
}

// add a variable to the token table 
bool symbol_table::addvar(const std::string& name, const std::string& label){
    if(inScope){
        auto [ it, added ] = 
            local.emplace(name, token{current_type, name, line_no, label});
        if(added) printStatus(VAR_DECL, it->second);
        else { printError(VAR_REDEF, it->second); return false; } 
    } else {
        auto [ it, added ] = 
            global.emplace(name, token{current_type, name, line_no, label});
        if(added) printStatus(VAR_DECL, it->second);
        else { printError(VAR_REDEF, it->second); return false; } 
    }
    return true; 
}


//check if a variable has been initialized
bool symbol_table::usevar(const std::string& name){
    auto it = local.find(name);
    if(it != local.end()) printStatus(LOCAL_VAR_USE, it->second);
    else if(it = global.find(name); it != global.end()){
        if(it->second.isFunc){
            printError(USE_FUNC_AS_VAR, it->second,  name); 
            return false; 
        }
        printStatus(GLOBAL_VAR_USE, it->second);
    }
    else { printError(BAD_VAR, token(), name); return false; }
    return true; 
}

// add a function declaration 
bool symbol_table::addfunc( const std::string& name, 
                            const std::string& param,
                            const std::string& label,
                            const bool defined)
{
    auto [ it, added ] = 
        global.emplace(name, token{current_type, name, line_no, 
                label, true, param, defined});
    if(!added) { 
        if(it->second.type != current_type || it->second.param != param){
            printError(FUNC_REDECL, it->second); return false; 
        }
        else if(defined) it->second.defined = true; 
        /*
        if(it->second.defined){
            printError(FUNC_REDEF, it->second); return false; } 
        else if(defined){
            it->second.defined = true;
        } else {
            printError(FUNC_REDECL, it->second); return false; } 
        */
    }
    else if(defined) printStatus(FUNC_DEF, it->second);
    else printStatus(FUNC_DECL, it->second);
    return true; 
}

bool symbol_table::definefunc(const std::string& name, 
                              const std::string& param,
                              const std::string& label)
{
    return addfunc(name, param, label, true);
}

// check if a function is defined 
bool symbol_table::callfunc(const std::string& name) const {
    auto it = global.find(name);
    if(it != global.end()){ 
        if(!it->second.isFunc){ 
            printError(USE_VAR_AS_FUNC, it->second, name); 
            return false; 
        }
        printStatus(FUNC_CALL, it->second);
    } else { printError(BAD_CALL); return false; } 
    return true; 
}

void symbol_table::enterScope(){
    inScope = true; 
}

/* TODO: recycle memory locations for local objects */ 
void symbol_table::exitScope(){
    inScope = false;
    local.clear();
}

void symbol_table::printError(const int type, const token& tok,
                              const std::string& name) const 
{
    switch(type){
        case VAR_REDEF:
            std::cerr  << "Error:" << line_no << ':' 
                       << (inScope ? " local" : " global")
                       << " variable " << tok.toString()
                       << " already declared on line " << tok.line << '\n';
            break;
        case FUNC_REDECL:
            std::cerr  << "Error:" << line_no << ": function " 
                       << tok.toString()
                       << " already declared on line " << tok.line << '\n';
            break;
        case FUNC_REDEF:
            std::cerr  << "Error:" << line_no << ": function " 
                       << tok.toString()
                       << " already defined on line " << tok.line << '\n';
            break;
        case BAD_CALL:
        case BAD_VAR:
            std::cerr  << "Error:" << line_no << ": unknown "
                       << ((type == BAD_VAR) ? "variable " : "function ")
                       << name << '\n';
			break;
        case USE_VAR_AS_FUNC:
            std::cerr  << "Error:" << line_no << ": trying to use variable "
                       << name << " as a function\n";
			break;
        case USE_FUNC_AS_VAR:
            std::cerr << "Error:" << line_no << ": try to use function "
                      << name << " as a variable\n";
            break;
        case NO_MAIN:
            std::cerr << "Error:" << line_no << ": No main found\n"; 
    }
}

void symbol_table::printStatus(const int type, const token& tok) const { 
    switch(type){
        case VAR_DECL:
            std::cout << (inScope ? "Local " : "Global ") 
                      << tok.toString() << " variable declared on line "
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
}

void symbol_table::operator ++ (){ ++line_no; }
int  symbol_table::getlineno() const { return line_no; }

const token *symbol_table::operator [] (const std::string& name) const {
    auto it = local.find(name);
    if(it == local.end()) 
        if((it = global.find(name)) == global.end()) return nullptr;
    return &(it->second);
}

