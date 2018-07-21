#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int MultFirst = 0;
int MultSec = 0;

double FirstMethod(double, int);
double SecondMethod(double, unsigned int);

int main (void)
{
	
	double Result1,Result2;
	int n;
	while(1){
		printf("Introduza o n:");
		scanf("%d",&n);
	
		Result1 = FirstMethod(0.5, n);
		Result2 = SecondMethod(0.5, n);

	
		printf ("Resultado1 = %5f N. de mult1 = %d Resultado2 = %5f N. de mult2 = %d\n", Result1, MultFirst, Result2, MultSec);
		
		MultFirst = 0;
		MultSec = 0;
	}
	exit (EXIT_SUCCESS);
}

double FirstMethod (double x, int n)
{
	if (n == 0) 
		return  1;
	else{
		MultFirst++;
		return (x * FirstMethod(x, n-1));
	}
}

double SecondMethod (double x, unsigned int n)
{
	double result, resultanx;
	
	if (n == 0) 
		return result = 1;
	else{
		if (n % 2 == 0){
			resultanx = SecondMethod(x, n/2);
			result = resultanx * resultanx;
			MultSec++;
		}else{
			resultanx = SecondMethod(x, n/2);
			result = x * resultanx * resultanx;
			MultSec+=2;			
		}
	}
	return result;
}

