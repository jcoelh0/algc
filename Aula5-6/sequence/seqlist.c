/*******************************************************************************
 Ficheiro de implementa��o do Tipo de Dados Abstracto SEQ_LIST (seqlist.c).
 A estrutura de dados de suporte da sequ�ncia � uma estrutura, constitu�da pelos
 campos de tipo inteiro Size para indicar o n�mero de elementos armazenados na
 sequ�ncia e os campos de tipo ponteiro para n�s de lista biligada Head e Tail,
 para representar, respectivamente, a cabe�a e a cauda da lista biligada onde
 s�o armazenados os n�meros inteiros.

 Autor : Jo�o Manuel Andr� Coelho                         NMEC : 80335        
*******************************************************************************/

#include <stdio.h> 
#include <stdlib.h>

#include "seqlist.h"  /* Ficheiro de interface do TDA - ADT Interface file */

/************ Defini��o da Estrutura de Dados Interna da Sequ�ncia ************/

typedef struct binode *PtBiNode;
struct binode /* defini��o do n� da lista biligada */
{
	int Elem; /* o elemento da lista */
	PtBiNode PtPrev, PtNext; /* ponteiros para o n�s anterior e seguinte */
};

struct seqlist
{
  int Size; /* n�mero de elementos - sequence's size */
  PtBiNode Head; /* ponteiro para o in�cio da lista (cabe�a da lista) - list head */
  PtBiNode Tail; /* ponteiro para o fim da lista (cauda da lista) - list tail */
};

/*********************** Controlo Centralizado de Error ************************/

static unsigned int Error = OK;  /* inicializa��o do erro */

static char *ErrorMessages[] = {
                                 "sem erro - Without Error",
                                 "sequencia(s) inexistente(s) - Sequence(s) do not exist",
                                 "memoria esgotada - Out of memory",
                                 "indice errado - Wrong index",
                                 "elemento inexistente - Element does not exist",
                                 "sequencia vazia - Empty sequence",
                                 "sequencia cheia - Full sequence",
                                 "dimensao da sequencia errada - Wrong size",
                                 "ficheiro inexistente - File does not exist",
                                 "ponteiro nulo - Null pointer"
                               };

static char *AbnormalErrorMessage = "erro desconhecido - Unknown error";

/************** N�mero de mensagens de erro previstas no m�dulo ***************/

#define N (sizeof (ErrorMessages) / sizeof (char *))

/******************** Prot�tipos dos Subprogramas Internos ********************/

PtBiNode BiNodeCreate (int);
void BiNodeDestroy (PtBiNode *);
void DoubleListDestroy (PtBiNode *);

/*************************** Defini��o das Fun��es ****************************/

void SeqListClearError (void)
{ Error = OK; }

int SeqListError (void)
{ return Error; }

char *SeqListErrorMessage (void)
{
  if (Error < N) return ErrorMessages[Error];
  else return AbnormalErrorMessage;  /* sem mensagem de erro - no error message */
}

PtSeqList SeqListCreate ()
{
	PtSeqList SeqL;
	if((SeqL = malloc (sizeof (struct seqlist))) == NULL){ 
		Error = NO_MEM; 
		return NULL; 	
	}
	PtBiNode pont;
	if((pont= malloc(sizeof(struct binode))) == NULL){
		Error = NO_MEM;
		return NULL;	
	}else{
		SeqL -> Size =0;
		SeqL -> Head = pont;
		SeqL -> Tail = pont;		
		Error = OK;
		return SeqL;	
	}
}

void SeqListDestroy (PtSeqList *pseq)
{
	PtSeqList Tmp = *pseq;

	/* verifica se o sequencia existe - verifies if sequenc exists */
	if (Tmp == NULL) { Error = NO_SEQ; return ; }

	/* liberta��o da mem�ria din�mica - free dynamic memory */
	free (Tmp);

	Error = OK;
	*pseq = NULL;  /* programa��o defensiva - defensive programming */
}

PtSeqList SeqListCopy (PtSeqList pseq)
{
  PtSeqList Copy;

  /* verifica se a sequ�ncia existe - verifies if sequence exists */
  if (pseq == NULL) { Error = NO_SEQ; return NULL; }

  /* cria��o da sequ�ncia copia nulo - creating an empty sequence */
  if ((Copy = SeqListCreate ()) == NULL) return NULL;

  /* fazer a copia da sequ�ncia - copying the components */
  for (PtBiNode Node = pseq->Head; Node != NULL; Node = Node->PtNext)
  {
    SeqListInsert (Copy, Node->Elem);
    if (Error == NO_MEM) break;
  }
  
  if (Error == NO_MEM) { SeqListDestroy (&Copy); return NULL; }
  
  Copy->Size = pseq->Size;
  Error = OK;
  return Copy;  /* devolve a sequ�ncia copia - returning the new sequence */
}

PtSeqList SeqListFileCreate (char *pfname)
{
  PtSeqList Seq; FILE *PtF; unsigned int Size, I; int Elem;

  /* abertura com validacao do ficheiro para leitura - opening the text file for reading */
  if ( (PtF = fopen (pfname, "r")) == NULL) { Error = NO_FILE; return NULL; }

  /* leitura da dimens�o da sequ�ncia e do n�mero de elementos */
  /* reading the sequence's dimension and the number of elements */
  fscanf (PtF, "%u", &Size);
  if (Size < 1) { Error = BAD_SIZE; fclose (PtF); return NULL; }

  /* cria��o da sequ�ncia vazia - creating an empty sequence */
  if ((Seq = SeqListCreate ()) == NULL) { fclose (PtF); return NULL; }

  Error = OK;
  /* leitura da sequ�ncia do ficheiro - reading the sequence's components from the text file */
  for (I = 0; I < Size; I++)
  {
    fscanf (PtF, "%d", &Elem);
    SeqListInsert (Seq, Elem);
    if (Error == NO_MEM) break;
  }

  if (Error == NO_MEM) { SeqListDestroy (&Seq); return NULL; }

  fclose (PtF);  /* fecho do ficheiro - closing the text file */
  return Seq;  /* devolve a sequ�ncia criada - returning the new sequence */
}

void SeqListStoreFile (PtSeqList pseq, char *pfname)
{
  FILE *PtF;

  /* verifica se a sequ�ncia existe - verifies if sequence exists */
  if (pseq == NULL) { Error = NO_SEQ; return ; }

  /* verifica se a sequ�ncia tem elementos - verifies if sequence has elements */
  if (pseq->Size == 0) { Error = SEQ_EMPTY; return ; }

  /* abertura com validacao do ficheiro para escrita - opening the text file for writing */
  if ((PtF = fopen (pfname, "w")) == NULL) { Error = NO_FILE; return ; }

  /* escrita do n�mero de elementos armazenados na sequ�ncia */
  /* writing the number of elements */
  fprintf (PtF, "%u\n", pseq->Size);

  /* escrita da sequ�ncia - writing the sequence's components in the text file */
  for (PtBiNode Node = pseq->Head; Node != NULL; Node = Node->PtNext)
	fprintf (PtF, "%d\n", Node->Elem);

  Error = OK;
  fclose (PtF);  /* fecho do ficheiro - closing the text file */
}

int SeqListGetSize (PtSeqList pseq)
{
	if(pseq == NULL){
		Error = NO_SEQ;
		return 0;
  	}else{
		Error = OK;
	return pseq -> Size;
  }
}

int SeqListGetElement (PtSeqList pseq, int pindex)
{
	if(pseq == NULL){
		Error = NO_SEQ;
  	}
  	int size = pseq -> Size;
  	if((pindex >= 0  && pindex < size) || (pindex <= -1 && pindex > -size)){
		Error = OK;
		if(pindex < 0){
			int ind=0;
			PtBiNode aux1 = pseq -> Tail;
			while(ind != pindex){
				aux1 = aux1 -> PtPrev;
				ind--;	
			};
		return aux1 -> Elem;

		}else{
			int ind=0;
			PtBiNode aux2 = pseq -> Head;
			while(ind != pindex){
				aux2 = aux2 -> PtNext;
				ind++;	
			};
			return aux2 -> Elem;
		}
	}else{
		Error = BAD_INDEX;
		return 0;
	}
}

void SeqListSetElement (PtSeqList pseq, int pindex, int pvalue)
{
	if(pseq == NULL){
		Error = NO_SEQ;
		return;
  	}
  	int size = pseq -> Size;
  	if((pindex >= 0  && pindex < size) || (pindex <= -1 && pindex > -size)){
		Error = OK;
		if(pindex < 0){
			int ind=0;
			PtBiNode aux1 = pseq -> Tail;
			while(ind != pindex){
				aux1 = aux1 -> PtPrev;
				ind--;	
			};
			aux1 -> Elem = pvalue;

		}else{
			int ind=0;
			PtBiNode aux2 = pseq -> Head;
			while(ind != pindex){
				aux2 = aux2 -> PtNext;
				ind++;	
			};
			aux2 -> Elem= pvalue;
		}
	}else{
		Error = BAD_INDEX;
		return;
	}
}

int SeqListEquals (PtSeqList pseq1, PtSeqList pseq2)
{
	if(pseq1 == NULL || pseq2 == NULL){
		Error = NO_SEQ;
  	}
  	Error = OK;
  	if(pseq1 -> Size != pseq2 -> Size){
		return 0;
  	}else{
		int i;	
		int validate =1;
		int size = pseq1 -> Size;
		PtBiNode aux1 = pseq1 -> Head;
		PtBiNode aux2 = pseq2 -> Head;
		for(i=0;i<size;i++){		
			if(aux1 -> PtNext == NULL || aux2 -> PtNext == NULL){	
				if(aux1 -> PtNext == NULL && aux2 -> PtNext == NULL){
					break;
				}
			}else{	
				if(aux1 -> Elem != aux2 -> Elem){
					validate= 0;			
					break;
				}else{
					aux1 = aux1 -> PtNext;
					aux2 = aux2 -> PtNext;
				}
			}
		}
		return validate;
	}
}

void SeqListInsert (PtSeqList pseq, int pvalue)
{
	if(pseq == NULL){
		Error = NO_SEQ;
		return;
  	}
  	PtBiNode pont;
  	if((pont = BiNodeCreate(pvalue)) != NULL){
		Error = OK;
		int size = pseq -> Size; 
		if( size == 0){
			pseq -> Head = pont;
			pseq -> Tail = pont;
			pseq -> Size = size + 1;
		}else{
			pont -> PtPrev = pseq -> Tail;		
			(pseq-> Tail) -> PtNext = pont;
			pseq -> Tail = pont;
			pseq -> Size = size + 1; 
  		}
	}
}

void SeqListDelete (PtSeqList pseq, int pvalue)
{
	if(pseq == NULL){
  		Error = NO_SEQ;
		return;
  	}
  	int size = pseq -> Size;
  	int validate = 0;
  	if(size==0){
		Error = SEQ_EMPTY;
		return;
  	}else{
		Error = OK;
		int i;
		PtBiNode aux = pseq -> Head;

		for(i=0;i<size;i++){
			if(aux -> Elem == pvalue){
				(aux -> PtPrev) -> PtNext = aux -> PtNext;		
				validate = 1;
				pseq -> Size -=1;
				break;
			}
			aux = aux -> PtNext;
		}
		if(validate == 0){
			Error = NO_NUMBER;
			return;
		}
	}
}

int SeqListIsOrdered (PtSeqList pseq)
{
	if(pseq == NULL){
		Error = NO_SEQ;
		return 0;
  	}

  	int size = pseq -> Size;
  	if(size == 0){
		Error = SEQ_EMPTY;
		return 0;
  	}
  	int validate =1;
  	int i;
  	PtBiNode aux = pseq -> Head;
  	PtBiNode aux2;
  	for(i=0;i<size-1;i++){
		aux2 = aux -> PtNext;
		if((aux -> Elem ) > (aux2 -> Elem )){
			validate =0;
			break;
		}
		aux = aux -> PtNext;
  	}
	return validate;
}

int SeqListIsEvenOdd (PtSeqList pseq)
{
	if(pseq == NULL){
		Error = NO_MEM;
		return 0;
  	}
  	int size = pseq -> Size;
  	if(size == 0){
		Error = SEQ_EMPTY;
		return 0;
  	}else{
		PtBiNode aux1 = pseq -> Head;
		PtBiNode aux2 = pseq -> Head;
		int validate1=1;
		int validate2=1;
		int i;
		if((aux1 -> Elem) % 2 == 0){			
			for(i=0;i<size;i++){
				if(i%2==0){
					if((aux1 -> Elem)%2 !=0){
						validate1 =0;
						break;				
					}			
				}
				aux1 = aux1 -> PtNext;
			}
			for(i=0;i<size;i++){
				if(i%2!=0){
					if((aux2 -> Elem)%2 ==0){
						validate2 =0;
						break;				
					}			
				}
				aux2 = aux2 -> PtNext;
			}
		}else{
			for(i=0;i<size;i++){
				if(i%2==0){
					if((aux1 -> Elem)%2 ==0){
						validate1 =0;
						break;				
					}			
				}
				aux1 = aux1 -> PtNext;
			}
			for(i=0;i<size;i++){
				if(i%2!=0){
					if((aux2 -> Elem)%2 !=0){
						validate2 =0;
						break;				
					}			
				}
				aux2 = aux2 -> PtNext;
			}
		}

		if(validate1 ==1 && validate2 == 1){
			return 1;
	  	}else{
			return 0;
 	 	}
	}
}

int SeqListNumberOfMultiples (PtSeqList pseq, int pvalue)
{
	if(pseq == NULL){
  		Error = NO_SEQ;
		return 0;
  	}
  	int size = pseq -> Size;
  	if(size == 0){
  		Error = SEQ_EMPTY;
		return 0;
  	}else{
		int count=0;
		PtBiNode aux= pseq-> Head;
		int i;
		for(i=0;i< size;i++){
			if(((aux -> Elem) % pvalue) == 0)
				count +=1;
			aux = aux -> PtNext;
		}
		return count;
	}
}

void SeqListSmallerBigger (PtSeqList pseq, int * psmall, int * pbig)
{
	if(pseq == NULL){
  		Error = NO_SEQ;
		return;
  	}
  	int size = pseq -> Size;
  	if(size == 0){
  		Error = SEQ_EMPTY;
		return;
  	}else{
		int i,j;
		*psmall = (pseq -> Head) -> Elem;
		*pbig = (pseq -> Head) -> Elem;
		PtBiNode aux1 = pseq -> Head;
		PtBiNode aux2 = pseq -> Head;
		for(i=0;i< size;i++){
			if((aux1 -> Elem) < *psmall)
				*psmall = aux1 -> Elem;
			aux1 = aux1 -> PtNext;
		}
		for(j=0;j< size;j++){
			if((aux2 -> Elem) > *pbig){
				*pbig = aux2 -> Elem;
			}
			aux2 = aux2 -> PtNext;
		}
	}
}

int SeqListIsPalindromic (PtSeqList pseq)
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
  	PtSeqList L1 = SeqListCreate();
  	PtSeqList L2 = SeqListCreate();
  	PtBiNode aux1 = pseq -> Head;
  	PtBiNode aux2 = pseq -> Tail;
  	int i,j;
  	for(i=0;i<size;i++){
		SeqListInsert(L1, aux1->Elem);
		aux1 = aux1 -> PtNext;
  	}
  	for(j= size-1;j>=0;j--){
		SeqListInsert(L2, aux2->Elem);
		aux2 = aux2 -> PtPrev;
  	}
  	return SeqListEquals(L1,L2);
}

double SeqListAverage (PtSeqList pseq)
{
	if(pseq == NULL){
		Error = NO_SEQ;
		return 0;
  	}
  	int size = pseq -> Size;
  	if(size == 0){
		Error = SEQ_EMPTY;
		return 0;
  	}else{
		Error = OK;
		double sum=0;
		int i;
		PtBiNode aux = pseq -> Head;
		for(i=0;i< size;i++){
			sum+= aux -> Elem;
			aux = aux -> PtNext;
		}
		return sum/i;
  	}
}

/*******************************************************************************
 Fun��o auxiliar para criar um n� da lista biligada. Valores de erro: OK ou NO_MEM.
 
 Auxiliary function to create a binode. Error codes: OK or NO_MEM.
*******************************************************************************/

PtBiNode BiNodeCreate (int pelem)	/* aloca��o do n� */
{
	PtBiNode Node;

	if ((Node = (PtBiNode) malloc (sizeof (struct binode))) == NULL)
	{ Error = NO_MEM; return NULL; }

	Node->Elem = pelem;	/* copiar a informa��o */
	Node->PtPrev = NULL;	/* apontar para detr�s para NULL */
	Node->PtNext = NULL;	/* apontar para a frente para NULL */

	Error = OK;
	return Node;
}

/*******************************************************************************
 Fun��o auxiliar para libertar um n� da lista biligada. Valores de erro: OK ou NULL_PTR.
 
 Auxiliary function to free a binode. Error codes: OK or NULL_PTR.
*******************************************************************************/
void BiNodeDestroy (PtBiNode *pnode)	/* liberta��o do n� */
{
	if (*pnode == NULL) { Error = NULL_PTR; return; }
	free (*pnode);	/* liberta��o do n� */
	*pnode = NULL;	/* colocar o ponteiro a nulo */
	Error = OK;
}

/*******************************************************************************
 Fun��o auxiliar para destruir uma lista biligada. Valores de erro: OK ou NULL_PTR.
 
 Auxiliary function to destroy a double link list. Error codes: OK or NULL_PTR.
*******************************************************************************/
void DoubleListDestroy (PtBiNode *phead)
{
	PtBiNode TmpHead = *phead; PtBiNode Node;

	if (TmpHead == NULL) { Error = NULL_PTR; return; }
	while (TmpHead != NULL)
	{
		Node = TmpHead; TmpHead = TmpHead->PtNext;
		BiNodeDestroy (&Node);
	}
	Error = OK; 
}
