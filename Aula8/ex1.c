#include <stdio.h>

int nmult1=0, nmult2=0;

int rec(int);
int din(int);

int main(void)
{
	for(int i=0; i<13; i++)
	{
		nmult1 = 0;
		int res1 = rec(i);
		printf("N=%2d  Rec: %8d  nMult1: %4d ||",i, res1, nmult1);
		nmult2 = 0;
		int res2 = din(i);
		printf("Din: %8d  nMult2: %4d \n", res2, nmult2);
	}
}

int rec(int n)
{
	int val = 0;
	if(n==0) return 1;
	else if(n == 1){
		int aux1 = rec(n-1);
		return aux1 + aux1;
	}
	else{
		int aux2 = rec(n-1);
		val = aux2 + aux2 + aux2;
		for(int i=1; i<n/2; i++){
			nmult1 += 2;
			val += 2 * rec(i) * rec(n-i-1);
		}
		if(n % 2 == 1){
			nmult1++;
			int aux = rec(n/2);
			val += aux * aux;
		}	
	}
	return val;
}

int din(int n)
{
	int res[n+1];
	res[0] = 1;
	int sum;
	for(int k=1; k<=n; k++){
		sum = res[k-1];
		for(int i=0; i<k; i++){
			nmult2++;
			sum += res[i]*res[k-i-1];
		}
		res[k] = sum;
	}
	return res[n];
}
