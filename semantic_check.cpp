#include <iostream>
#include <string>

using namespace std;


void operation_check(const char *left, const char *right, symbol_table& table, int line){
	//cout << "checking 2 variables " << left << " " << right << " on line " << line << "\n";
	string ltype, rtype;
	ltype = table.getType(left);
	rtype = table.getType(right);
	cout << left << ":" << ltype << " " << right << ":" << rtype << endl;
	if(ltype == rtype) return;
	cout << "Type error on line " << line << ": Cannot mix " << left << " of type "  << ltype << " and " << right << " of type " << rtype << "\n"; 
}
void operation_check(const char *left, const int right, symbol_table& table, int line){
	//cout << "checking variable and literal " << left << " and " << right << " on line " << line << "\n";
	string var_type = table.getType(left);
	if(var_type == "int") return;
	cout << "Type error on line " << line << ": " << left << " expected " << var_type << " but recieved int\n"; 
}
void operation_check(const char *left, const float right, symbol_table& table, int line){
	//cout << "checking variable and literal " << left << " and " << right << "\n";
	string var_type = table.getType(left);
	if(var_type == "float") return;
	cout << "Type error on line " << line << ": " << left << " expected " << var_type << " but recieved float\n"; 
}
void function_check(const char *func, const char *param, symbol_table& table, int line){
	string param_type = table.getparam(func);
	string var_type = table.getType(param);
	if(param_type == var_type) return;
	cout << "Type error on line " << line << ": Function " << func << " expected " << param_type << " but instead recieved " << var_type << "\n";
}
