#include "token_table.hpp" 

token::token(const std::string& type, const std::string& name, const int line, 
        const std::string& ret = "", const std::string& param = "",
        defined = false):
            type(type), name(name), line(line), ret(ret), param(param), 
            defined(defined) {}

void token_table::decl_type(const char *type){
    current_type = type;
}

int addvar(const char *name, const int line_no){
    /* if local is empty, we're in global space. */ 
    if(local.empty()){
        if(global.count(name)) return -1;
        global[name] = token(current_type, name, line_no);
        return 0;
    } else {
        if(local.top().count(name)) return -1;
        local.top()[name] = token(current_type, name, line_no);
        return 0;
    }
}

int token_table::addfunc(const char *name, const char *param, const int line_no){
    if(global.count(name)) return -1;
    global[name] = token(current_type, name, param, line_no);
    return 0;
}

int token_table::definefunc(const char *name, const char *param, const int line_no){
    if(global.count(name)){
        if(global[name].defined) return -1;
        global[name].defined = true;
        return 0;
    } else {

