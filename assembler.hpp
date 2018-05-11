/* sic compiler for CS-UY 3943 by Nikita Georgiou and Jack Martinez */ 

#pragma once

#include "labler.hpp"
#include "symbol_table.hpp" 

#include <string> 
#include <vector> 
#include <iostream> 
#include <fstream> 
#include <sstream> 
#include <array>

class assembler {
public:
    assembler();

    template <typename... Args> inline void addins(const Args&... args); 

    void read(const char type);
    void writes(const std::string& str);
    void writeExpr(const char type);

    void startloop(); 
    void endloop(); 
    void startif();
    void endif();
    void branch(const char type);
    void addelse(const char type);
    void boollhs(const char type); 
    void boolrhs(const std::string& name, const char type); 

    void call(const std::string& name);
    void binop(const char op, const std::string& F);
    void eval(const char type);
    inline bool isint(const token *tok);
    void assign(const std::string& name); 
    void ret();
    void funcprologue(const std::string& fname, const std::string& pname);
    void bindmain();
    void addendifmain(const std::string& name);
    void end(); 

    void addvars(); 
    void addvarlist(const std::string& name);
    void clearvarlist(); 

    void addterm(const std::string&);
    void clearexpr();

    // functions to interface with the symbol table 
    std::string getType(const std::string& name);
    std::string getparam(const std::string& name);
    void decl_type(const std::string& name);
    bool addvar(const std::string& name);
    bool usevar(const std::string& name);
    bool addfunc(const std::string& name, 
                 const std::string& param, 
                 const bool defined = false);
    bool definefunc(const std::string& name, const std::string& param);
    bool callfunc(const std::string& name) const;
    void enterScope();
    void exitScope();
    int getlineno();
    void operator ++ (); 

    void printcode() const; 
    void outputcode(); 
    void output_asm(); 
    void error_encountered(const char *what);
private:
    bool error;  // dont generate assembly if weve encountered even one error
    labler memno;   // for variables
    labler labelno; // for functions 
    symbol_table t; 
    // dummy instruction to be replaced by main in bindmain() 
    std::vector<std::string> code = { "DUMMY\n" };
    std::stringstream ins; 
    std::string currentFunc; 
    std::vector<std::string> varlist;
    std::vector<std::string> expr; 
    std::array<std::string, 3> boolops; 
};
