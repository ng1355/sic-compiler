int main(int dummy){
	int a,b,temp;
	write "give me 2 positive numbers";
	read a,b;
	
	if(b > a){
		temp = a;
		a = b;
		b = temp;
	}

	/* a <= b from this point on */
	while(b > 0){
		temp = a - b * (a/b); /* integer division; so get remainder */
		a = b;
		b = temp;
	}
	write a;
}
