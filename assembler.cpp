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

// main doesnt require a return statement so we add one
void assembler::addretifmain(const std::string& name){
   if(name == "main") ret();
   else return;
}

void assembler::addvars(){
    for(const auto& s : varlist){ 
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

    size_t pos = 0;
    std::string s(str);
    std::string token;
    const std::string delimiter("\\n");
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        if(token.size()){
            token.insert(0, "\"");
            token.append("\"");
            addins("WRITES", token);
            addins("NEWLINE");
        }
        s.erase(0, pos + delimiter.length());
    }
    if(s.size()){
        s.insert(0, "\"");
        s.append("\"");
        addins("WRITES", s);
    }
}

void assembler::writeExpr(const char type){
	if(error) return;
    std::string F((type == 'f') ? "F" : "");
    addins("POP" + F, memno.getnew());
    addins("WRITE" + F, memno.getlast());
}

void assembler::branch(const char type){
	if(error) return;
    std::string F((type == 'f') ? "F" : "");
    std::string boolop(boolops[1]);
    if(boolop == "==")
        addins("JNE" + F, boolops[0], boolops[2], labels.top());
    else if(boolop == "!=")
        addins("JEQ" + F, boolops[0], boolops[2], labels.top());
    else if(boolop == "<")
        addins("JGE" + F, boolops[0], boolops[2], labels.top());
    else if(boolop == ">")
        addins("JLE" + F, boolops[0], boolops[2], labels.top());
    else if(boolop == "<=")
        addins("JGT" + F, boolops[0], boolops[2], labels.top());
    else if(boolop == ">=")
        addins("JLT" + F, boolops[0], boolops[2], labels.top());
}

void assembler::addelse(const char type){
	if(error) return;
    std::string F((type == 'f') ? "F" : "");
    std::string boolop(boolops[1]);
    labels.emplace(labelno.getnew());
    if(boolop == "==")
        addins("JEQ" + F, boolops[0], boolops[2], labels.top());
    else if(boolop == "!=")
        addins("JNE" + F, boolops[0], boolops[2], labels.top());
    else if(boolop == "<")
        addins("JLT" + F, boolops[0], boolops[2], labels.top());
    else if(boolop == ">")
        addins("JGT" + F, boolops[0], boolops[2], labels.top());
    else if(boolop == "<=")
        addins("JLE" + F, boolops[0], boolops[2], labels.top());
    else if(boolop == ">=")
        addins("JGE" + F, boolops[0], boolops[2], labels.top());
}

void assembler::startloop(){
	if(error) return;
    std::string jmpout(labelno.getnew()); //jumps out of loop
    std::string loop(labelno.getnew());   //label before loop
    addins("LABEL", loop);
    labels.emplace(jmpout);
    labels.emplace(loop);
    labels.emplace(jmpout);
}

void assembler::endloop(){
	if(error) return;
    //code.pop_back(); // ugly. compensates for startif() 
    addins("JUMP", labels.top());
    labels.pop();
    addins("LABEL", labels.top()); // not sure if always true...
    labels.pop();
}

void assembler::cleanloop(){ if(error) return; labels.pop(); } 

void assembler::startif(){
    if(error) return;
    labels.emplace(labelno.getnew());
}

void assembler::endif(){
	if(error) return;
    addins("LABEL", labels.top());
    labels.pop();
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
    addins("PUSH" + F, memno.getlast());
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

void assembler::addterm(const std::string& term, const char type){
    if(type == 's'){
        usevar(term);
    }
    if(error) return;
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
    usevar(name);
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
    addins("LABEL", func->label);
    // main is an exception and doesn't pop an argument
    if(fname != "main") addins("POP", param->label);
}

void assembler::bindmain() {
    const token *massmain = t["main"];
    if(massmain){ 
        code[2] += massmain->label + "\n"; 
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
    if(error) return;
    std::cout << "Code:\n"; 
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
    if(debug) std::cerr << "Assembler error: " << what << '\n';
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

bool assembler::usevar(const std::string& name) { 
    bool success = t.usevar(name); 
    if(!success) error_encountered("usevar");
    return success;
}

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


