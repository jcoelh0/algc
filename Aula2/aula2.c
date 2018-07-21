#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "elapsed_time.h"


/* defini��o do tipo de dados inteiro sem sinal de 64 bits */
/* definition of the 64-bit unsigned integer type */
typedef unsigned long long u64;

/* alus�o das fun��es que implementam os algoritmos pretendidos */
/* allusion to the functions that implement the required algorithms */
u64 fibRec (int);
u64 fibRep (int);
u64 fibFechada (int);
u64 fibFechada2 (int);
u64 fibFechada3 (int);

/* vari�vel global para contar as opera��es aritm�ticas executadas pelo algoritmo */
/* global variable for counting the arithmetic operations executed by the algorithm */
int Count;

int main (void)
{
	int NLines, N, Test;
	u64 Result;
	/* leitura do valor limite da tabela */
	/* reading the limit value for the table */
	do
	{
		printf ("N de elementos da tabela? "); Test = scanf ("%d", &NLines);
		scanf ("%*[^\n]"); scanf ("%*c");
	} while (Test == 0);


	(void)elapsed_time();
	
    /* impress�o da tabela de execu��o do algoritmo */
    /* impression of the table values for the algorithm's execution */
	for (N = 1; N <= NLines; N++)
	{
        /* inicializa��o da vari�vel global contadora das opera��es aritm�ticas */
        /* initialization of the global variable for counting the arithmetic operations */
		Count = 0;
		
		
		/* invoca��o do algoritmo pretendido */
		/* invocation of the pretended algorithm */
		Result = fibFechada3 (N);

		/* apresenta��o do resultado e do n�mero de opera��es aritm�ticas executadas pelo algoritmo */
		/* presenting the result and the number of arithmetic operations executed by the algorithm */
		fprintf (stdout, "Fibonacci de %2d = %12lld e executou %10d somas\n", N, Result, Count);
	}
	double dt = elapsed_time();
	
	fprintf (stdout, "Demorou %f segundos\n", dt);
	
	exit (EXIT_SUCCESS);
}

/* implementa��o dos algoritmos pretendidos */
/* acrescentar a contagem das opera��es aritm�ticas executadas pelos algoritmos usando a vari�vel global */

/* implementation of the pretended algorithms */
/* do not forget to count the arithmetic operations using the global variable */

u64 fibRec(int n) // solu��o recursiva
{
	int result;
	if(n < 2)
	{
		result = n;
	}
	else
	{
		result = fibRec(n-1)+fibRec(n-2);
		Count++;
	}
	return result;
}

u64 fibRep (int n) // solu��o repetitiva
{
	u64 atual, anterior = 1, anterior2 = 0;
	if(n < 2)
	{
		atual = n;
	}
	else
		for(int i = 2; i <= n; i++)
		{
			atual = anterior + anterior2;
			anterior2 = anterior;
			anterior = atual;
			Count++;
		}
	return atual;
}

u64 fibFechada (int n)
{
	double phi = (1 + sqrt(5))/2;
	Count++;
	u64 result;
	for(int i = 0; i <= n; i++)
	{
		result = (pow(phi, (double) n) - pow((1-phi), (double) n))/sqrt(5);
		Count++;
	}
	return result;	
}

u64 fibFechada2 (int n)
{
	double phi = (1 + sqrt(5))/2;
	u64 result;
	for(int i = 0; i <= n; i++)
	{
		result = round((pow(phi, (double) n))/sqrt(5));
		Count++;
	}
	return result;	
}

u64 fibFechada3 (int n)
{
	double c1 = 0.44721357549995773928, c2 = 0.48121182505960344750;
	u64 result;
	for(int i = 0; i <= n; i++)
	{
		result = round(c1 * exp( (double) n * c2));
		Count++;
	}
	return result;	
}
