/* testing control flow */
int main(int dummy){
	int i,j;
	j = 5;
	write "j = 5";
	read i;
	if(i > j){
		write "i > j";
	}else if(i < j){
		write "i < j";
	}else{
		write "i == j";
	}
}
