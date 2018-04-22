#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

//Checks to make sure that 2 variables are of the same type
void operation_check(const char *left, const char *right, symbol_table& table, int line){
	//cout << "checking 2 variables " << left << " " << right << " on line " << line << "\n";
	string ltype, rtype;
	ltype = table.getType(left);
	rtype = table.getType(right);
	//cout << left << ":" << ltype << " " << right << ":" << rtype << endl;
	if(ltype == rtype) return;
	cout << "Type error on line " << line << ": Cannot mix " << left << " of type "  << ltype << " and " << right << " of type " << rtype << "\n"; 
}
//check if a variable and an int-literal are of the same type
void operation_check(const char *left, const int right, symbol_table& table, int line){
	//cout << "checking variable and literal " << left << " and " << right << " on line " << line << "\n";
	string var_type = table.getType(left);
	if(var_type == "int") return;
	cout << "Type error on line " << line << ": " << left << " expected " << var_type << " but recieved int\n"; 
}
//check if a variable and a float literal are of the same type 
void operation_check(const char *left, const float right, symbol_table& table, int line){
	cout << "checking variable and literal " << left << " and " << right << "\n";
	string var_type = table.getType(left);
	if(var_type == "float") return;
	cout << "Type error on line " << line << ": " << left << " expected " << var_type << " but recieved float\n"; 
}

//check if a function is being passed in the correct type parameter
void function_check(const char *func, const char *param, symbol_table& table, int line){
	string param_type = table.getparam(func);
	string var_type = table.getType(param);
	if(param_type == var_type) return;
	cout << "Type error on line " << line << ": Function " << func << " expected " << param_type << " but instead recieved " << var_type << "\n";
}

//check to make sure that boolean values are calculated using the same types
void boolean_check(vector<char*> vlist, vector<int> ilist, vector<float> flist, symbol_table& table, int line){
	/*
		variables
		ints
		floats
	*/
	
	//check if we have both ints and floats, if we do it's a type mismatch
	if(ilist.size() > 0 && flist.size() > 0){
		cout << "Type mismatch on line " << line << ": Can't mix types int and float\n";
		return;
	}
	string type, previous;
	int isize = ilist.size();
	int fsize = flist.size();
	for(int i = 0; i < vlist.size(); i++){
		type = table.getType(vlist[i]);
		if((type == "int" && fsize > 0) || type == "float" && isize > 0)
			cout << "Type mismatch on line " << line << ": Can't mix types int and float\n";
		if(type != previous && previous != "")
			cout << "Type mismatch on line " << line << ": Can't mix types int and float\n";
	}
}
