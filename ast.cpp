#include "ast.hpp"

Node::Node() = default;
Node::Node(	NodeType type, 
			const double value
		  ):type(type), value(value),left(NULL),right(NULL){}
//Fix later
/*Node::~Node(){
	std::cout << "Node with value " << value << " being deleted\n";
	if(left) delete left;
	if(right) delete right;
}
*/
void Node::insertLeft(Node* l){
	left = l;
}
void Node::insertRight(Node* r){
	right = r;
}

int main(){
	std::cout << "Garbage :c\n";
	Node a = Node(IntValue, 1);
	Node b = Node(IntValue, 2);
	Node c = Node(IntValue, 3);
	Node d = Node(IntValue, 4);
	Node e = Node(IntValue, 5);

	a.insertLeft(&b);
	a.insertRight(&c);

//	b.insertLeft(&d);
//	b.insertRight(&e);
}

