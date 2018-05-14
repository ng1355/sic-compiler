/* sic compiler for CS-UY 3943 by Nikita Georgiou and Jack Martinez */ 

#include "assembler.hpp" 

assembler::assembler() {}

template <typename... Args> 
inline void assembler::addins(const Args&... args){ 
    ((ins << args << ' '), ...);
    std::string finalins(ins.str());
    finalins.back() = '\n';
    code.emplace_back(finalins);
    ins.str(""); 
}

void assembler::addendifmain(const std::string& name){
   if(name == "main") end();
   else return;
}

void assembler::addvars(){
    for(const auto& s : varlist){ 
        std::cout << "adding var " << s << '\n';
        addvar(s);
    }
    varlist.clear();
}

void assembler::addvarlist(const std::string& name){
    varlist.emplace_back(name);
}

void assembler::clearvarlist() { varlist.clear(); } 

void assembler::read(const char type){
	if(error) return;
    std::string F((type == 'f') ? "F" : "");
    for(const auto& s : varlist){
		if(!usevar(s)){
			error_encountered("read");
			return;
		}
        addins("READ" + F, t[s]->label);
	}
    varlist.clear();
}

void assembler::writes(const std::string& str){
	if(error) return;
    addins("WRITES", "\"",str, "\"");
    addins("NEWLINE");
}

void assembler::writeExpr(const char type){
	if(error) return;
    std::string F((type == 'f') ? "F" : "");
    addins("WRITE" + F, memno.getlast());
    addins("NEWLINE");
}

void assembler::branch(const char type){
	if(error) return;
    std::string F((type == 'f') ? "F" : "");
    std::string boolop(boolops[1]);
    if(boolop == "==")
        addins("JNE" + F, boolops[0], boolops[2], labelno.getlast());
    else if(boolop == "!=")
        addins("JEQ" + F, boolops[0], boolops[2], labelno.getlast());
    else if(boolop == "<")
        addins("JGE" + F, boolops[0], boolops[2], labelno.getlast());
    else if(boolop == ">")
        addins("JLE" + F, boolops[0], boolops[2], labelno.getlast());
    else if(boolop == "<=")
        addins("JLT" + F, boolops[0], boolops[2], labelno.getlast());
    else if(boolop == ">=")
        addins("JGT" + F, boolops[0], boolops[2], labelno.getlast());
}

void assembler::addelse(const char type){
	if(error) return;
    std::string F((type == 'f') ? "F" : "");
    std::string boolop(boolops[1]);
    ++labelno; 
    if(boolop == "==")
        addins("JEQ" + F, boolops[0], boolops[2], labelno.getlast());
    else if(boolop == "!=")
        addins("JNE" + F, boolops[0], boolops[2], labelno.getlast());
    else if(boolop == "<")
        addins("JGT" + F, boolops[0], boolops[2], labelno.getlast());
    else if(boolop == ">")
        addins("JGT" + F, boolops[0], boolops[2], labelno.getlast());
    else if(boolop == "<=")
        addins("JLE" + F, boolops[0], boolops[2], labelno.getlast());
    else if(boolop == ">=")
        addins("JGE" + F, boolops[0], boolops[2], labelno.getlast());
}

void assembler::startloop(){
	if(error) return;
	addins("//starting loop");
    addins("LABEL", labelno.getnew());
}

void assembler::endloop(){
	if(error) return;
    code.pop_back(); // ugly. compensates for endif() 
    addins("JUMP", labelno.getlasti() - 1);
    addins("LABEL", labelno.getlast()); // not sure if always true...
}

void assembler::startif(){
    ++labelno; 
}

void assembler::endif(){
	if(error) return;
    addins("LABEL", labelno.getlast());
}

void assembler::boollhs(const char type){
	if(error) return;
    std::string F((type == 'f') ? "F" : "");
    addins("POP" + F, memno.getnew());
    boolops[0] = memno.getlast();
}

void assembler::boolrhs(const std::string& sign, const char type){
	if(error) return;
    std::string F((type == 'f') ? "F" : "");
    addins("POP" + F, memno.getnew());
    boolops[1] = sign;
    boolops[2] = memno.getlast();
}

void assembler::call(const std::string& name){
	if(error) return;
    addins("CALL", t[name]->label);
    addins("POP", memno.getlast());
}

void assembler::binop(const char op, const std::string& F){
	if(error) return;
    switch(op){
        case '+':
            addins("ADD" + F, memno.getlast(), memno.getlasti() - 1, memno.getnew());
            break;
        case '-':
            addins("SUB" + F, memno.getlast(), memno.getlasti() - 1, memno.getnew());
            break;
        case '*':
            addins("MUL" + F, memno.getlast(), memno.getlasti() - 1, memno.getnew());
            break;
        case '/':
            addins("DIV" + F, memno.getlast(), memno.getlasti() - 1, memno.getnew());
            break;
    }
    //addins("PUSH" + F, memno.getlast());
}

void assembler::eval(const char type){
	if(error) return;
    std::string F((type == 'i') ? "" : "F");
    for(const auto& term : expr) {
        if(std::isdigit(term[0]))
            addins("PUSH" + F, "#" + term);
        else if(std::isalpha(term[0]))
            addins("PUSH" + F, t[term]->label);
        else {
            addins("POP", memno.getnew());
            addins("POP", memno.getnew());
            binop(term[0], F);
        }
    }
    expr.clear();
}

void assembler::addterm(const std::string& term){
    expr.emplace_back(term);
}

void assembler::clearexpr(){
    expr.clear();
}

inline bool assembler::isint(const token *tok){
    return tok->type == "int";
}

void assembler::assign(const std::string& name){
    if(error) return; 
    const token *var = t[name];
    addins("POP", var->label);
}

void assembler::ret(){
	if(error) return;
    const token *func = t[currentFunc];
    if(isint(func)) addins("PUSH", memno.getlast());
    else addins("PUSHF", memno.getlast());
    addins("RETURN");
}

void assembler::funcprologue(const std::string& fname, const std::string& pname){
	if(error) return;
    const token *func = t[fname];
    const token *param = t[pname]; 
    currentFunc = fname; 
    addins("label", func->label);
    // main is an exception and doesn't pop an argument
    if(fname != "main") addins("POP", param->label);
}

void assembler::bindmain() {
    const token *massmain = t["main"];
    if(massmain){ 
        code[0] = "START " + massmain->label + '\n';
        return; 
    }
    t.printError(NO_MAIN); 
    error_encountered("bindmain"); 
}

void assembler::end() {
    if(error) return;
    addins("STOP"); 
}

void assembler::printcode() const {
    for(const auto& s : code) std::cout << s; 
}

void assembler::outputcode() {
    if(error) return; 
    std::fstream output("output.mass", std::fstream::out | std::fstream::binary);
    if(!output.is_open()){
        std::cerr << "Failed to open output file for writing\n"; 
        return;
    }
    for(const auto& s : code) output << s; 
    output.close();
}

void assembler::error_encountered(const char *what) {
    std::cerr << "Assembler error: " << what << '\n';
    error = true;
}

/* symbol table interafaces */ 

std::string assembler::getType(const std::string& name){
    return t.getType(name);
}

std::string assembler::getparam(const std::string& name){
    return t.getparam(name);
}

void assembler::decl_type(const std::string& name) { 
    t.decl_type(name); 
}

/* associates variable with memory address */ 
bool assembler::addvar(const std::string& name){
    if(!t.addvar(name, memno.getnew())){ --memno; error_encountered("addvar"); }
    if(error) return false; 
    return true; 
}

bool assembler::usevar(const std::string& name) { return t.usevar(name); } 

/* associates function with label */ 
bool assembler::addfunc(const std::string& name, 
             const std::string& param, 
             const bool defined){
    if(!t.addfunc(name, param, labelno.getnew(), defined)){ --labelno; error_encountered("addfunc"); }
    if(error) return false; 
    return true; 
}

bool assembler::definefunc(const std::string& name, const std::string& param){
    return t.definefunc(name, param, labelno.getnew());
}

bool assembler::callfunc(const std::string& name) const {
    return t.callfunc(name);
}
int assembler::getlineno() {
    return t.getlineno(); 
}

void assembler::operator ++ () { ++t; } 

void assembler::enterScope() { t.enterScope(); } 
void assembler::exitScope() { t.exitScope(); } 


