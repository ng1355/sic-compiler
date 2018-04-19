#include <string>
#include <iostream>

enum NodeType{
	Undefined,
	OP_Assign,
	OP_GT,	//greater than
	OP_GE,	//greater than or equal to
	OP_LT,	//less than
	OP_LE,	//less than or equal to
	OP_EQ,	//equality
	OP_Plus,
	OP_Minus,
	OP_Mul,
	OP_Div,
	IntValue,
	FloatValue,
	KW_Int,
	KW_Float,
	KW_If,
	KW_Else,
	KW_While,
	KW_Read,
	KW_Write,
	LPAR,
	RPAR,
	LBrace,
	RBrace,
	Semicolon,
	Comma
};

class Node{
public:
	Node();
	Node(NodeType type,const double value);
	//~Node();
	
	void insertLeft(Node* l);
	void insertRight(Node* r);
private:
	NodeType type;
	double value;
	Node* left;
	Node* right;	
};

/*
class pgm{};
class decl{};
class kind{};
class var_list{};
class var_list_opt{};
class func_decl{};
class func_def{};
class body{};
class body_decl{};
class body_stmt{};
class stmt{};
class while_stmt{};
class write_expr_lst{};
class wel_optional{};
class wel_group{};
class factor{};
class bool_exp{};
class func_call{};
class term{};
class term_mulop{};
class expr1{};
class expr1_addop{};
class mulop{};
class addop{};
class bool_op{};
class expr{};
*/
