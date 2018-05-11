// #sic compiler for CS-UY 3943 by Nikita Georgiou and Jack Martinez

#include "semantic_check.hpp"

//Checks to make sure that 2 variables are of the same type
void operation_check(const char *left, const char *right){
	std::cout << "a\n";
	std::string ltype, rtype;
	ltype = mass.getType(left);
	rtype = mass.getType(right);
    int line = mass.getlineno();
	if(ltype == rtype) return;
	std::cout << "Type error on line " << line << ": Cannot mix " << left 
         << " of type "  << ltype << " and " << right << " of type " 
         << rtype << "\n"; 
    mass.error_encountered("var check");
}

//check if a variable and an int-literal are of the same type
void operation_check(const char *left, const int right){
	std::cout << "b\n";
	std::string var_type = mass.getType(left);
    int line = mass.getlineno();
	if(var_type == "int") return;
	std::cout << "Type error on line " << line << ": " << left << " expected " 
              << var_type << " but recieved int\n"; 
    mass.error_encountered("int check");
}

//check if a variable and a float literal are of the same type 
void operation_check(const char *left, const float right){
	std::cout << "c\n";
	std::string var_type = mass.getType(left);
    int line = mass.getlineno();
	if(var_type == "float") return;
	std::cout << "Type error on line " << line << ": " << left << " expected " 
              << var_type << " but recieved float\n"; 
    mass.error_encountered("float check");
}

//check if a function is being passed in the correct type parameter
void function_check(const char *func, const char *param){
	std::string param_type = mass.getparam(func);
	std::string var_type = mass.getType(param);
    int line = mass.getlineno();
	if(param_type == var_type) return;
	std::cout << "Type error on line " << line << ": Function " << func 
              << " expected " << param_type << " but instead recieved " 
              << var_type << "\n";
    mass.error_encountered("param check");
}
void function_check(const char *func, const int param){
	std::string param_type = mass.getparam(func);
	int line = mass.getlineno();
	if(param_type == "int") return;
	std::cout << "Type error on line " << line << ": Function " << func 
              << " expected " << param_type << " but instead recieved " 
              << " an int literal\n";
    mass.error_encountered("param check");

}
void function_check(const char *func, const float param){
	std::string param_type = mass.getparam(func);
	int line = mass.getlineno();
	std::cout << param_type << " | float literal\n";
	if(param_type == "float") return;
	std::cout << "Type error on line " << line << ": Function " << func 
              << " expected " << param_type << " but instead recieved " 
              << "a float literal(" << param << ")\n";
    mass.error_encountered("param check");

}

/* checks if what's being returned agrees with the function's return type.
 * Currently relies on globals too much... */ 
void return_check(){
    if(ret_type != expr_type){ 
        std::cerr << "Type error on line " << mass.getlineno()
                  << ": return statement does not match function return type\n";
        mass.error_encountered("ret check");
        printf("ret: %c expr: %c\n", ret_type, expr_type); 
    }
}

//check to make sure that boolean values are calculated using the same types
void boolean_check(std::vector<char*> vlist, std::vector<int> ilist, std::vector<float> flist){
    int line = mass.getlineno();
	//check if we have both ints and floats, if we do it's a type mismatch
	if(ilist.size() > 0 && flist.size() > 0){
		std::cerr << "Type mismatch on line " << line 
                  << ": Can't mix types int and float\n";
        mass.error_encountered("bool check");
		return;
	}
	std::string type, previous;
	int isize = ilist.size();
	int fsize = flist.size();
	for(int i = 0; i < vlist.size(); i++){
		type = mass.getType(vlist[i]);
		if((type == "int" && fsize > 0) || (type == "float" && isize > 0)){
			std::cerr << "Type mismatch on line " << line 
                      << ": Can't mix types int and float\n";
            mass.error_encountered("bool check");
            return;
        }
		if(type != previous && previous != ""){
			std::cerr << "Type mismatch on line " << line 
                      << ": Can't mix types int and float\n";
            mass.error_encountered("bool check");
            return;
        }
	}
}
