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
	
	while(cursor!=NULL)
	{
		p = cursor->next;
		freeNodeArray(cursor);
		cursor = p;
	}
}

void freeType(types type, void * pData)
{
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
		case wrapper_t:
			wrapperFree((pWrapper)(pData));
			break;
		case array_t:
			freeArray((pArray)(pData));
			break;
		case list_t:
			listFree((pList)(pData));
			break;
		default:
			fprintf(stderr, "Le type de la variable n'est pas connu : impossible de liberer l'espace memoire associe\n");
			exit(-1);
			break;
	}
}

void freeNodeArray(pArray node)
{
	if(node==NULL)
		return;
		
	freeType(node->type, node->data);
	free(node);
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


void removeElemArray(pArray previous_elem)
{
	if(previous_elem==NULL)
		return;
	
	pArray cursor = previous_elem->next;//element to remove
	previous_elem->next = cursor->next;
	freeNodeArray(cursor);
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
 pPetri petriCreateNode(int * places, unsigned int num_pl, int * trans, unsigned int num_tr)
 {
	pPetri p = (pPetri)malloc(sizeof(petri));
    assert(p);
    
    p->places = places;
    p->num_pl = num_pl;
    p->trans = trans;
    p->num_tr =num_tr;

    return p; 
 }
 
 void petriFree(pPetri p)
 {
	 if(p==NULL)
		return;
	
	 free(p->places);
	 free(p->trans);
	 free(p);
 }
 
 
 
 
 
 /*********************************************************************
 * 
 * 					
 * 						GRAPH FUNCTIONS
 * 
 * 
 *********************************************************************/

pList listCreate(pArray * p, unsigned int size)
{
	pList output = (pList)malloc(sizeof(list));
	assert(output);
	
	output->data = p;
	output->size = size;
	
	return output;
}

void listFree(list * p)
{
	if(p==NULL)
		return;
		
	freeList(p->data, p->size);
	free(p);
}




 /*********************************************************************
 * 
 * 					
 * 						WRAPPER FUNCTIONS
 * 
 * 
 *********************************************************************/

void wrapperFree(pWrapper p)
{
	if(p==NULL)
		return;
	
	freeType(p->type, p->data);
	free(p);
}

pArray  wrapperAddToList(pArray  w_list, unsigned int id, types type, void * data)
{
	pWrapper c = (pWrapper)malloc(sizeof(wrapper));
	assert(c);
	
	c->type = type;
	c->id = id;
	c->data = data;
	
	w_list = add(w_list, wrapper_t, c);
	
	return w_list;
}

int wrapperGetId(pArray p)
{
	/*given an element of the list of wrapper return the id of the wrapper*/
	if(p==NULL || p->data==NULL)
		return -1;
	
	pWrapper c = (pWrapper)(p->data);
	
	return c->id;
}


pWrapper wrapperGetElem(pArray w_list, unsigned int id)
{
	/* given the id of the wrapper return the wrapper or NULL
	 * if there is no such wrapper
	*/
	pArray cursor = w_list;
	
	while(cursor && ((pWrapper)(cursor->data))->id!=id)
	{
		cursor = cursor->next;
	}
	
	if(cursor==NULL)
	{
		return NULL;
	}
	
	return (pWrapper)(cursor->data);
}

pArray  wrapperRemoveFromList(pArray w_list, unsigned int id)
{
	if(w_list==NULL)
		return w_list;
	
	pArray cursor = w_list;
	pArray p_cursor = NULL;
	int c_id;
	
	while(cursor && (c_id=wrapperGetId(cursor))!=id)//get the wrapper
	{
		p_cursor = cursor;//previous element
		cursor = cursor->next;
	}
	
	if(c_id==-1 || cursor==NULL)
	{
		return w_list;
	}
	
	if(p_cursor==NULL)//first element of the list of wrapper
	{
		cursor = w_list->next;
		freeNodeArray(w_list);
		w_list = cursor;
		
		return w_list;
	}
	
	p_cursor->next = cursor->next;
	freeNodeArray(cursor);//free the wrapper node
	
	return w_list;
}

pArray wrapperFreeList(pArray w_list)
{
	freeArray(w_list);
	return NULL;
}
