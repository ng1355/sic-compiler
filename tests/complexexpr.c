/*trying complex expressions*/
int square(int val){
    int ret; 
    write "square got: ", val;
	ret = val * val;
    write "square is: ", ret;
    return ret;
}
int cube(int val){
    int ret; 
    write "cube got: ", val;
	ret = val * val * val;
    write "cube is: ", ret;
    return ret;
}
int main(int dummy){
	int a,b,c,d;
	write "Give me 4 values!";
	read a,b,c,d;
	write square(a + b * 10) - cube(c * d) * square(a - b);
}
