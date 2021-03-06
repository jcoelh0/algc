#include<stdio.h>
#include <stdlib.h>

int nmult1 = 0, nmult2 = 0;
int fib1(int);
int fib2(int);

int main(void){
	
	for(int i=0; i<=12;i++){
		printf("N = %2d",i);
		nmult1 = 0; 
		nmult2 = 0;
		double result1 = fib1(i);
		printf("%10.0f	", result1);
		printf("Num mult1 = %5d", nmult1);
		double result2 = fib2(i);
		printf("%10.0f	", result2);
		printf("Num mult2 = %5d   \n", nmult2);
	}	
}

int fib1(int n){
	if(n == 0) return 0;
	else if(n == 1) return 1;
	else{
		nmult1 += 2;
		return (3*fib1(n-1) + 2*fib1(n-2));
	}
}

int fib2(int n){
	int a[n+1];
	a[0] = 0;
	a[1] = 1;
	if(n < 2) return a[n];
	for(int i=2;i<=n; i++){
		nmult2 += 2;
		a[i] = (3*a[i-1] + 2*a[i-2]);
	}
	return a[n];
}

