#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* alusão das funções que implementam os algoritmos pretendidos */
/* allusion to the functions that implement the required algorithms */
int armstrong ();


/* variável global para contar as operações aritméticas executadas pelo algoritmo */
/* global variable for counting the arithmetic operations executed by the algorithm */
int Count;

int main (void)
{

	armstrong ();


	exit (EXIT_SUCCESS);
}

/* implementação dos algoritmos pretendidos */
/* acrescentar a contagem das operações aritméticas executadas pelos algoritmos usando a variável global */

/* implementation of the pretended algorithms */
/* do not forget to count the arithmetic operations using the global variable */


int armstrong()
{
	int i, cent, dez, uni;
	for (i = 100; i <= 999; i++)
	{
		cent = i / 100;
		dez = i / 10;
		uni = i;
    	
    	cent = cent % 10;
    	dez = dez % 10;
    	uni = uni % 10;
    	double sum = pow(cent,3)+pow(dez,3)+pow(uni,3);
		if(i == sum)
		{
			fprintf (stdout, "%i\n", i);
			Count++;
		}
	}

	return 0;
}
