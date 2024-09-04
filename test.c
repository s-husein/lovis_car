#include <stdio.h>
void tostr(int num){
	char str[3];
	for(int i = 10, x = 0; i>=1; i/=10, x++){
		str[x] = (unsigned char)(num/i)+'0';
		num %= i;
	}
	str[3] = '\0';
	printf("%s", str);
}
int main(){
	int num = 24;
	tostr(num);
return 0;
}
