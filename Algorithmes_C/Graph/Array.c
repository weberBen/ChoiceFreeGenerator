#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Array.h"

 

/*********************************************************************
 * 
 * 					
 * 						LINKED LIST FUNCTIONS
 * 
 * 
 *********************************************************************/

pArray createNode(types type, void * data)
{
    pArray node = (pArray)malloc(sizeof(array));
    assert(node);

	node->type = type;
    node->data = data;
    node->next = NULL;

    return node;
}

pArray add(pArray l, types type, void * data)
{
    pArray node = createNode(type, data);
    node->next = l;

    return node;
}

unsigned int lengthArray(pArray p)
{
	unsigned int count =0;
	
	while(p)
	{
		count++;
		p = p->next;
	}
	
	return count;
}

void initializeList(void * a[], unsigned int n)
{
	unsigned int i;
	
	for(i=0; i<n; i++)
	{
		a[i] = NULL;
	}
}

void freeArray(pArray p)
{
	pArray cursor = p;
	types type;
	void * pData;
	
	while(cursor!=NULL)
	{
		type = cursor->type;
		pData = cursor->data;
		
		switch(type)
		{
			case uInt_t:
				uIntFree((pInt)(pData));
				break;
			case place_t:
				placeFree((pPlace)(pData));
				break;
			case transition_t:
				transitionFree((pTransition)(pData));
				break;
			case petri_t:
				petriFree((pPetri)(pData));
				break;
			default:
				fprintf(stderr, "Le type de la variable n'est pas connu : impossible de liberer l'espace memoire associe\n");
				exit(-1);
				break;
		}
		
		p = cursor;
		cursor = cursor->next;
		free(p);
	}
}

void freeList(pArray a[], unsigned int n)
{
	if(a==NULL)
		return;
	
	unsigned int i;
	
	for(i=0; i<n; i++)
	{
		freeArray(a[i]);
	}
	
	free(a);
}


/*********************************************************************
 * 
 * 					
 * 						uInt FUNCTIONS
 * 
 * 
 *********************************************************************/
 
 pInt uIntCreateNode(unsigned int data)
 {
	pInt p = (pInt)malloc(sizeof(Int));
    assert(p);
    
    p->data = data;

    return p; 
 }
 
 unsigned int uIntValue(pArray p)
 {
	 /*    |type = uInt			  |
	  * p= |data -----------> pu= | data = value 
	  *    |?					  |
	 */
	 if(p==NULL || p->data==NULL)
	 {
		 fprintf(stderr, "uInt est nul, impossible de retourner une valeur !\n");
		 exit(-1);
	 }
	
	 pInt d = (pInt)(p->data);
	 
	 return d->data;
 }
 
 
 void uIntFree(pInt p)
 {
	 if(p==NULL)
		return;
	 
	 free(p);
 }
 

void initializeIntArray(int list[], unsigned int n, int value)
{
	unsigned int i;
	for(i=0; i<n; i++)
	{
		list[i] =  value;
	}
}


/*********************************************************************
 * 
 * 					
 * 						PLACE FUNCTIONS
 * 
 * 
 *********************************************************************/
 
 pPlace placeCreateNode(unsigned int label)
 {
	pPlace p = (pPlace)malloc(sizeof(place));
    assert(p);
    
    p->label = label;

    return p; 
 }
 
 void placeSetM(pPlace p, int M)
 {
	 if(p==NULL)
	 {
		 fprintf(stderr, "Impossible de mettre a jour une valeur d'un element nul\n");
		 exit(-1);
	 }
	 p->M = M;
 }
 
 
 unsigned int placeLabel(pArray p)
 {
	 if(p==NULL || p->data==NULL)
	 {
		 fprintf(stderr, "place est nul, impossible de retourner une valeur !\n");
		 exit(-1);
	 }
	 
	 pPlace d = (pPlace)(p->data);
	 return d->label;
 }
 
 int placeM(pArray p)
 {
	 if(p==NULL || p->data==NULL)
	 {
		 fprintf(stderr, "place est nul, impossible de retourner une valeur !\n");
		 exit(-1);
	 }
	 
	 pPlace d = (pPlace)(p->data);
	 return d->M;
 }
 
 
 void placeFree(pPlace p)
 {
	 if(p==NULL)
		return;
	
	 free(p);
 }
 
 

/*********************************************************************
 * 
 * 					
 * 						TRANSITION FUNCTIONS
 * 
 * 
 *********************************************************************/
 pTransition transitionCreateNode(pPlace input, pArray output)
 {
	pTransition p = (pTransition)malloc(sizeof(transition));
    assert(p);
    
    p->input = input;
    p->output = output;

    return p; 
 }
 
 void transitionSetWeigth(pTransition p, int in_w, int out_w)
 {
	 if(p==NULL)
	 {
		 fprintf(stderr, "Impossible de mettre a jour une valeur d'un element nul\n");
		 exit(-1);
	 }
	 p->in_weigth = in_w;
	 p->out_weigth = out_w;
 }
 
 
 pArray transitionOutput(pArray p)
 {
	 if(p==NULL || p->data==NULL)
	 {
		 fprintf(stderr, "transition est nul, impossible de retourner une valeur !\n");
		 exit(-1);
	 }
	 
	 pTransition d = (pTransition)(p->data);
	 return d->output;
 }
 
 pPlace transitionInput(pArray p)
 {
	 if(p==NULL || p->data==NULL)
	 {
		 fprintf(stderr, "transition est nul, impossible de retourner une valeur !\n");
		 exit(-1);
	 }
	 
	 pTransition d = (pTransition)(p->data);
	 return d->input;
 }
 
 int transitionInWeigth(pArray p)
 {
	 if(p==NULL || p->data==NULL)
	 {
		 fprintf(stderr, "transition est nul, impossible de retourner une valeur !\n");
		 exit(-1);
	 }
	 
	 pTransition d = (pTransition)(p->data);
	 return d->in_weigth;
 }
 
 int transitionOutWeigth(pArray p)
 {
	 if(p==NULL || p->data==NULL)
	 {
		 fprintf(stderr, "transition est nul, impossible de retourner une valeur !\n");
		 exit(-1);
	 }
	 
	 pTransition d = (pTransition)(p->data);
	 return d->out_weigth;
 }
 
 
 void transitionFree(pTransition p)
 {
	 if(p==NULL)
		return;
	 pTransition d = (pTransition)p;
	 
	 placeFree(d->input);
	 freeArray(d->output);
	 free(d);
 }

 
 
 /*********************************************************************
 * 
 * 					
 * 						PETRI FUNCTIONS
 * 
 * 
 *********************************************************************/
 pPetri petriCreateNode(pPlace node)
 {
	pPetri p = (pPetri)malloc(sizeof(petri));
    assert(p);
    
    p->node = node;
    p->input_t = NULL;
    p->output_t = NULL;

    return p; 
 }
 
 void petriSetInput(pPetri p, pArray input)
 {
	 assert(p);
	 
	 p->output_t = input;
 }
 
 void petriSetOutput(pPetri p, pArray output)
 {
	 assert(p);
	 
	 p->output_t = output;
 }
 
 void petriFree(pPetri p)
 {
	 if(p==NULL)
		return;
	
	 pPetri d = (pPetri)p;
	 
	 placeFree(d->node);
	 freeArray(d->input_t);
	 freeArray(d->output_t);
	 free(d);
 }
 
 
 
 
 
 /*********************************************************************
 * 
 * 					
 * 						GRAPH FUNCTIONS
 * 
 * 
 *********************************************************************/

void graphFree(graph * g)
{
	if(g==NULL)
		return;
		
	freeList(g->data, g->size);
	free(g);
}

