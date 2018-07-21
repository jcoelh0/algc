/*******************************************************************************
 Ficheiro de implementação do Tipo de Dados Abstracto SEQ_ARRAY (seqarray.c).
 A estrutura de dados de suporte da sequência é um registo, constituída pelos
 campos de tipo inteiro Dim para indicar a capacidade de armazenamento da 
 sequência e Size para indicar o número de elementos efectivamente armazenados na
 sequência e o campo de tipo ponteiro Seq, para representar a sequência (array) 
 atribuída dinamicamente onde são armazenados os números inteiros.

 Autor : João Manuel André Coelho                         NMEC : 80335          
*******************************************************************************/

#include <stdio.h> 
#include <stdlib.h>

#include "seqarray.h"  /* Ficheiro de interface do TDA - ADT Interface file */

/************ Definição da Estrutura de Dados Interna da Sequência ************/

struct seqarray
{
  unsigned int Dim; /* capacidade de armazenamento - array's size */
  int Size; /* número de elementos - sequence's size */
  int *Seq; /* ponteiro para armazenar a sequencia - array to store the numbers */
};

/*********************** Controlo Centralizado de Error ************************/

static unsigned int Error = OK;  /* inicialização do erro */

static char *ErrorMessages[] = {
                                 "sem erro - Without Error",
                                 "sequencia(s) inexistente(s) - Sequence(s) do not exist",
                                 "memoria esgotada - Out of memory",
                                 "indice errado - Wrong index",
                                 "elemento inexistente - Element does not exist",
                                 "sequencia vazia - Empty sequence",
                                 "sequencia cheia - Full sequence",
                                 "dimensao da sequencia errada - Wrong size",
                                 "ficheiro inexistente - File does not exist"
                               };

static char *AbnormalErrorMessage = "erro desconhecido - Unknown error";

/************** Número de mensagens de erro previstas no módulo ***************/

#define N (sizeof (ErrorMessages) / sizeof (char *))

/*************************** Definição das Funções ****************************/

void SeqArrayClearError (void)
{ Error = OK; }

int SeqArrayError (void)
{ return Error; }

char *SeqArrayErrorMessage (void)
{
  if (Error < N) return ErrorMessages[Error];
  else return AbnormalErrorMessage;  /* sem mensagem de erro - no error message */
}

PtSeqArray SeqArrayCreate (unsigned int pdim)
{
	if(pdim == 0){
		Error = BAD_SIZE;
		return NULL;
	}
	
	PtSeqArray pSeqArr;
	int* pArray;
	if((pSeqArr = malloc (sizeof (struct seqarray))) == NULL){ 
		Error = NO_MEM; 
		return NULL; 	
	}
	if((pArray = malloc(sizeof(int)*(pdim))) == NULL){
		Error = NO_MEM;
		free (pArray);
		pArray = NULL; 
		return NULL; 	
	
	}else{
		pSeqArr -> Dim = pdim;
		pSeqArr -> Size = 0;
		pSeqArr -> Seq = pArray;		
		Error = OK;
		return pSeqArr;	
	}
}

void SeqArrayDestroy (PtSeqArray *pseq)
{
	PtSeqArray TmpSeq = *pseq;

	/* verifica se a sequencia existe - verifies if sequenc exists */
	if (TmpSeq == NULL) { Error = NO_SEQ; return ; }

	/* libertação da memória dinâmica - free dynamic memory */
	free (TmpSeq);

	Error = OK;
	*pseq = NULL;  /* programação defensiva - defensive programming */
	
}

PtSeqArray SeqArrayCopy (PtSeqArray pseq)
{
  PtSeqArray Copy; int I;

  /* verifica se a sequência existe - verifies if sequence exists */
  if (pseq == NULL) { Error = NO_SEQ; return NULL; }

  /* criação da sequência copia vazia - creating an empty sequence */
  if ((Copy = SeqArrayCreate (pseq->Dim)) == NULL) return NULL;

  /* fazer a copia dos elementos da sequência - copying the components */
  for (I = 0; I < pseq->Size; I++) Copy->Seq[I] = pseq->Seq[I];
  Copy->Size = pseq->Size;

  return Copy;  /* devolve a sequência copia - returning the new sequence */
}

PtSeqArray SeqArrayFileCreate (char *pfname)
{
  PtSeqArray Seq; FILE *PtF; unsigned int Dim, Size, I;

  /* abertura com validacao do ficheiro para leitura - opening the text file for reading */
  if ( (PtF = fopen (pfname, "r")) == NULL) { Error = NO_FILE; return NULL; }

  /* leitura da dimensão da sequência e do número de elementos armazenados */
  /* reading the sequence's dimension and the number of elements */
  fscanf (PtF, "%u %u", &Dim, &Size);
  if (Dim < 1) { Error = BAD_SIZE; fclose (PtF); return NULL; }

  /* criação da sequência com a dimensão pretendida - creating the sequence */
  if ((Seq = SeqArrayCreate (Dim)) == NULL) { fclose (PtF); return NULL; }

  /* leitura da sequência do ficheiro - reading the sequence's components from the text file */
  for (I = 0; I < Size; I++) fscanf (PtF, "%d", Seq->Seq+I);
  Seq->Size = Size;

  fclose (PtF);  /* fecho do ficheiro - closing the text file */
  Error = OK;
  return Seq;  /* devolve a sequência criada - returning the new sequence */
}

void SeqArrayStoreFile (PtSeqArray pseq, char *pfname)
{
  FILE *PtF; unsigned int I;

  /* verifica se a sequência existe - verifies if sequence exists */
  if (pseq == NULL) { Error = NO_SEQ; return ; }

  /* verifica se a sequência tem elementos - verifies if sequence has elements */
  if (pseq->Size == 0) { Error = SEQ_EMPTY; return ; }

  /* abertura com validacao do ficheiro para escrita - opening the text file for writing */
  if ((PtF = fopen (pfname, "w")) == NULL) { Error = NO_FILE; return ; }

  /* escrita da dimensão da sequência e do número de elementos armazenados na sequência */
  /* writing the sequence's dimension and the number of elements */
  fprintf (PtF, "%u %u\n", pseq->Dim, pseq->Size);

  /* escrita da sequência - writing the sequence's components in the text file */
  for (I = 0; I < pseq->Size; I++) fprintf (PtF, "%d\n", pseq->Seq[I]);

  Error = OK;
  fclose (PtF);  /* fecho do ficheiro - closing the text file */
}

int  SeqArrayGetDimension (PtSeqArray pseq)
{
	if(pseq == NULL){
		Error = NO_SEQ;	
		return 0;	
  	}else{
		Error = OK;
		return pseq -> Dim;
  	}
}

int SeqArrayGetSize (PtSeqArray pseq)
{
	if(pseq == NULL){
		Error = NO_SEQ;	
		return 0;	
  	}else{
		Error = OK;
		return pseq -> Size;
  	}
}

int SeqArrayGetElement (PtSeqArray pseq, int pindex)
{
	if(pseq == NULL){
		Error = NO_SEQ;
  	}
  	int size = pseq -> Size;
  	if((pindex >= 0  && pindex < size) || (pindex <= -1 && pindex > -size)){
		Error = OK;
		if(pindex < 0){
			return pseq -> Seq[size+pindex];
		}else{  	
			return pseq -> Seq[pindex];
  		}
  	}else{
		Error = BAD_INDEX;
		return 0;
  	}
}

void SeqArraySetElement (PtSeqArray pseq, int pindex, int pvalue)
{
	if(pseq == NULL){
		Error = NO_SEQ;
		return;
	}
  	int size = pseq -> Size;
  	if((pindex >=0  && pindex < size)|| (pindex <= -1 && pindex > -size)){
		Error = OK;  	
		if(pindex < 0){
			pseq -> Seq[size+pindex] = pvalue;
		}else{  	
			pseq -> Seq[pindex] = pvalue;
  		}
  	}else{
		Error = BAD_INDEX;
		return;
  	}
}

int SeqArrayEquals (PtSeqArray pseq1, PtSeqArray pseq2)
{
	if(pseq1==NULL || pseq2 == NULL){
		Error = NO_SEQ;
		return 0;
  	}
  	Error = OK;
  	int Dim1 = pseq1 -> Dim;
  	int Dim2 = pseq2 -> Dim;
  	int Size1 = pseq1 -> Size;
  	int Size2 = pseq2 -> Size;
  	if((Dim1 != Dim2) || (Size1 != Size2)){
  		return 0;
  	}else{
		int i;	
		for(i=0;i<Size1;i++){		
			if(pseq1 -> Seq[i] != pseq2-> Seq[i]){
				return 0;			
				break;
			}
		}
		return 1;
  	}
}

void SeqArrayInsert (PtSeqArray pseq, int pvalue)
{
	if(pseq == NULL){
  		Error = NO_SEQ;
		return;
  	}
  	int size = pseq -> Size;
  	int dim = pseq -> Dim;
  	if(size == dim){
		Error = SEQ_FULL;
		return;
  	}else{
	  	Error = OK;
	  	int newsize = size + 1;
	  	pseq -> Seq[size] = pvalue;
	  	pseq -> Size = newsize;
	}
}

void SeqArrayDelete (PtSeqArray pseq, int pvalue)
{
	if(pseq == NULL){
  		Error = NO_SEQ;
		return;
  	}
  	int size = pseq -> Size;
  	int dim = pseq -> Dim;
  	int validate = 0;
  	if((dim - size) == dim){
		Error = SEQ_EMPTY;
		return;
  	}else{
		Error = OK;
	  	int newsize = size - 1;
	  	int i;
	  	for(i=0;i<size;i++){
	  		if((pseq -> Seq[i])==pvalue){
				pseq -> Seq[i] = 0;//tentar free(Seq[i])  
				pseq -> Size = newsize;
				validate=1;
				break;					
			}	
  	  	}
	  
	  	if(validate==0){
			Error = NO_NUMBER;
	  	}
	}
}

void SeqArrayDuplicate (PtSeqArray pseq)
{
  int *newSeq;
  /* verifica se a sequência existe - verifies if sequence exists */
  if (pseq == NULL) { Error = NO_SEQ; return; }
  
  /* verifica se a sequência não está vazia - verifies if sequence is not empty */  
  if (pseq->Size == 0) { Error = SEQ_EMPTY; return; }
 
  if ((newSeq = calloc (2*pseq->Size, sizeof (int))) == NULL)
  { Error = NO_MEM; return; }
  
  pseq->Dim = 2*pseq->Size;  /* armazenamento da nova dimensão - storing the new dimension */

  /* copiar os elementos - copying the elements */
  for (int I = 0; I < pseq->Size; I++) newSeq[I] = pseq->Seq[I];
  /* guardar o novo array e libertar o antigo - storing the new arry and freeing the old */
  int * Tmp = pseq->Seq; pseq->Seq = newSeq; free (Tmp);
  
  Error = OK;
}

int SeqArrayIsOrdered (PtSeqArray pseq)
{
	if(pseq == NULL){
		Error = NO_SEQ;
		return 0;
 	}

  	if((pseq -> Size) == 0){
  		Error = SEQ_EMPTY;
		return 0;
  	}

  	Error = OK;
  	int i;
  	int validate=1;
  	int size = pseq -> Size;
  	for(i=0;i<size-1;i++){
		if((pseq -> Seq[i+1]) < (pseq -> Seq[i]) ){
			validate = 0;
			break;
		}
  	}

  	return validate;
}

int SeqArrayIsEvenOdd (PtSeqArray pseq)
{
	if(pseq == NULL){
		Error = NO_SEQ;
		return 0;
  	}

  	if((pseq -> Size) == 0){
  		Error = SEQ_EMPTY;
		return 0;
  	}
  	Error = OK;
  	int validate1=1;
  	int validate2=1;
  	int size = pseq -> Size;
  	int i,j,k,z;
  	if((pseq -> Seq[0] % 2 )==0){
		for(i=0;i<size;i++){
			if( ((pseq -> Seq[i]) % 2 ) != 0){
				validate1 = 0;
				break;		
			}
			i++;	
		}

		for(j=1;j<size;j++){
			if( ((pseq -> Seq[i]) % 2 ) == 0){
				validate2 = 0;
				break;		
			}
			j++;	
		}
  	}else{
  		for(k=0;k<size;k++){
			if( ((pseq -> Seq[k]) % 2 ) == 0){
				validate1 = 0;
				break;		
			}
			k++;	
		}

		for(z=1;z<size;z++){
			if( ((pseq -> Seq[z]) % 2 ) != 0){
				validate2 = 0;
				break;		
			}
			z++;	
		}
  	}


  	if(validate1==1 && validate2==1){
		return 1;
  	}else{
  		return 0;
  	}
}

int SeqArrayNumberOfMultiples (PtSeqArray pseq, int pvalue)
{
	if(pseq == NULL){
		Error = NO_SEQ;
		return 0;
  	}
	
  	if((pseq -> Size) == 0){
  		Error = SEQ_EMPTY;
		return 0;
 	}
  	Error = OK;
  	int nMult=0;
  
  	int size = pseq -> Size;
  	int i;
  	for(i=0;i< size;i++){
  		if( ( pseq -> Seq[i] % pvalue ) == 0){
			nMult++;	
		}
  	}	

  	return nMult;
}

void SeqArraySmallerBigger (PtSeqArray pseq, int * psmall, int * pbig)
{
	if(pseq == NULL){
		Error = NO_SEQ;
		return;
  	}

  	if((pseq -> Size) == 0){
  		Error = SEQ_EMPTY;
		return;
  	}
  	Error = OK;
  	int size = pseq -> Size;
  	int i,j;
  	*psmall = pseq -> Seq[0];
  	*pbig = pseq -> Seq[0];
  	for(i=1;i<size;i++){
  		if(pseq -> Seq[i] < (*psmall)){
			*psmall = pseq -> Seq[i]; 
		}
  	}
  	for(j=1;j<size;j++){
  		if(pseq -> Seq[j] > (*pbig) ){
			*pbig = pseq -> Seq[j]; 
		}
  	}
}

int SeqArrayIsPalindromic (PtSeqArray pseq)
{
	if(pseq == NULL){
		Error = NO_SEQ;	
		return 0;
  	}
  	if((pseq -> Size) == 0){
  		Error = SEQ_EMPTY;
		return 0;
  	}
  	int size = pseq -> Size;
  	PtSeqArray aux1 = SeqArrayCreate(size);
  	PtSeqArray aux2 = SeqArrayCreate(size);
  	int i,j;
  	for(i=0;i<size;i++){
		SeqArrayInsert(aux1, pseq -> Seq[i]);
  	}
  	for(j= size-1;j>=0;j--){
		SeqArrayInsert(aux2, pseq -> Seq[j]);
  	}

  	return SeqArrayEquals(aux1,aux2);
}

double SeqArrayAverage (PtSeqArray pseq)
{
	if(pseq == NULL){
		Error = NO_SEQ;
		return 0;
  	}

  	if((pseq -> Size) == 0){
  		Error = SEQ_EMPTY;
		return 0;
  	}

  	Error = OK;
  	int size = pseq -> Size;
  	double sum=0;
  	double med;
 	int i;
  	for(i=0;i<size;i++)
		sum+= pseq -> Seq[i];

  	med = sum/size;
  	return med;
}
