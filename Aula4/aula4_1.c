#include <stdio.h>
#include <stdlib.h>

/* alus�o da fun��o que implementa o algoritmo pretendido */
/* allusion to the function that implements the algorithm */
int SequentialSequence (int [], int);

/* vari�vel global para contar as opera��es aritm�ticas executadas pelo algoritmo */
/* global variable for counting the arithmetic operations executed by the algorithm */
int Sum = 0;

int main (void)
{
	/* declara��o dos arrays de teste - usar o pretendido para cada execu��o */
	/* declaration of the test arrays - use each one for each execution */

	//int Array[] = { 1, 3, 4, 5, 5, 6, 7, 7, 8, 9 };
	int Array[] = { 1, 2, 4, 5, 5, 6, 7, 8, 8, 9 };
	//int Array[] = { 1, 2, 3, 6, 8, 8, 8, 9, 9, 9 };
	//int Array[] = { 1, 2, 3, 4, 6, 7, 7, 8, 8, 9 };
	//int Array[] = { 1, 2, 3, 4, 5, 7, 7, 8, 8, 9 };
	//int Array[] = { 1, 2, 3, 4, 5, 6, 8, 8, 9, 9 };
	//int Array[] = { 1, 2, 3, 4, 5, 6, 7, 9, 9, 9 };
	//int Array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 8, 9 };
	//int Array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 9 };
	//int Array[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
 
	//int Array[] = { };
	int NElem = sizeof (Array) / sizeof (int); int Result;

	/* invoca��o do algoritmo pretendido - algorithm invocation */ 
	Result = SequentialSequence (Array, NElem);

	/* apresenta��o do resultado e do n�mero de opera��es aritm�ticas executadas pelo algoritmo */
	/* presenting the result and the number of arithmetic operations executed by the algorithm */
	if (Result) fprintf (stdout, "Verifica ");
	else fprintf (stdout, "Nao verifica ");

	fprintf (stdout, "Resultado = %3d N. de operacoes = %3d\n", Result, Sum);

	exit (EXIT_SUCCESS);
}

/* implementa��o do algoritmo pretendido */
/* n�o se esque�a de contar as opera��es aritm�ticas executadas pelo algoritmo usando a vari�vel global */

/* implementation of the pretended algorithm */
/* do not forget to count the arithmetic operations using the global variable */

int SequentialSequence (int array[], int n)
{
	if (n < 2) return 0;
	
	for(int i = 1; i < n; i++)
	{
		Sum++;
		if(array[i] != array[i-1]+1)
			return 0;
	}
	return 1;
}

