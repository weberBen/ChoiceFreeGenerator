#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "Rand.h"
#include "Tools.h"

/***********************************************************************
 * 
 * 						RANDOM IN AN ARRAY
 * 
 **********************************************************************/
 
 /* All the functions help to pick random elements onces between two
  * integer values start and end (end excluded)
  * 
  * The functions create a custom element "container" that will store the
  * size of the linked list made of all the element between start and end, 
  * an unique id, the size of the array.
  * 
  * For example, if for the first time we need to get random value between 
  * 1 and 4 then we will have the following result :
  * 
  * 	* linked list of element between 1 and 3 :
  * ---------	  ---------	    ---------   
  * |		|	  |		  |     |		|
  * |	1	|---> |	  2	  |---> |	3	|
  * |		|	  |		  |     |		|
  * ---------	  ---------		---------
  * 	* the size of the linked list is then 3
  * 	* the id of the new element (liked list, size) is 0 (for example)
  * 
  * Then the result will be the following wrapper :
  * 	 -----------------------------------------------------
  * 	|		   ---------------------------------------	 |
  * 	|         |	---------	  ---------	    --------- |  |   
  * 	|		  | |		|	  |		  |     |		| |  |
  * 	|	* --> |	|	1	|---> |	  2	  |---> |	3	| |  |
  * 	|		  |	|		|	  |		  |     |		| |  |
  * 	|		  |	---------	  ---------		--------- |  |
  * 	|		   ---------------------------------------   |
  *     |													 |
  * 	|	* size = 3										 |
  * 	|	* id = 0										 |
  * 	|													 |
  * 	 -----------------------------------------------------
  * 
  * We use a wrapper to save multiple array at the same time (for example
  * if the user need to get random number from two different range)
  * 
  * Then all the wrappers are saved into a linked list
  * 
  *  ----------- 	  -----------
  * |			|	 |			 |
  * | wrapper 1 |--> | wrapper 2 | --> ...
  * |			|	 |			 |
  *  -----------	  -----------
  * 
  * the id of the wrapper is used to retrive the corresponding array initialize by the user
  * 
  * When a user get a new random value from an array we remove the current element
  * For example, if the array is [1,2,3] and we randomly pick the element at the position 1 (which here is 2), 
  * then we removed the element 2, update the array to get [1,3], decrease the size of the array by one and return the result
  */
  
  /********************************************************************
  * 						DEFINITION
  *********************************************************************/
typedef struct _container//wrapper
{
	pArray data;//linked list of element between start and end
	unsigned int size;//size of the linked list
	unsigned int id;//id of the wrapper
} container;

static pArray _list = NULL;//list of wrappers
static unsigned int _id = 0;//count of the id

  /********************************************************************
  * 						FUNCTIONS
  *********************************************************************/

static void freeContainer(pArray p)
{
	if(p==NULL || p->data==NULL)
		return;
	
	container * c = (container *)(p->data);
	freeArray(c->data);//free linked list of value between start and end
	free(c);//free the wrapper
	free(p);//free the element of linked list of wrapper
}

static int getId(pArray p)
{
	/*given an element of the list of wrapper return the id of the wrapper*/
	if(p==NULL || p->data==NULL)
		return -1;
	
	container * c = (container *)(p->data);
	
	return c->id;
}

static void freeStaticArray(unsigned int id)
{
	/* remove wrapper with the given id from the list of wrapper
	 * (and free the memory of all the inside element of the wrapper)
	*/
	if(_list==NULL)
		return;
	
	pArray cursor =_list;
	pArray p_cursor = NULL;
	int c_id;
	
	while(cursor && (c_id=getId(cursor))!=id)//get the wrapper
	{
		p_cursor = cursor;//previous element
		cursor = cursor->next;
	}
	
	if(c_id==-1 || cursor==NULL)
	{
		return;
	}
	
	if(p_cursor==NULL)//first element of the list of wrapper
	{
		cursor = _list->next;
		freeContainer(_list);
		_list = cursor;
		
		return;
	}
	
	p_cursor->next = cursor->next;
	freeContainer(cursor);//free the wrapper
}

static unsigned int createNodeStaticArray(unsigned int start, unsigned int end)
{
	/* given a start and an end create the wrapper and return the id of the wrapper*/
	int i;
	
	container * p =(container *)malloc(sizeof(container));//wrapper
	assert(p);
	pArray data = NULL;//linked list of elements between start and end
	
	 
	for(i=start; i<end; i++)
	{
		data = add(data, uInt_t, uIntCreateNode(i));//add value to the linked list
	}
	
	//initialize the wrapper
	p->data = data;
	p->size = end - start;//(end-1)-start+1
	p->id = _id;
	_id++;//increase the global count of the wrapper
	
	//add the wrapper into the list of wrapper
	_list = add(_list, 0, p);
	
	return p->id;//return the id of the wrapper
}

static container * getElem(unsigned int id)
{
	/* given the id of the wrapper return the wrapper or NULL
	 * if there is no such wrapper
	*/
	pArray cursor = _list;
	
	while(cursor && ((container *)(cursor->data))->id!=id)
	{
		cursor = cursor->next;
	}
	
	if(cursor==NULL)
	{
		return NULL;
	}
	
	return (container *)(cursor->data);
}


unsigned int randIni(int start, int end)
{ 
	/* user interface */
	
	int range = end - start;//(end-1)-start+1
	if(range==0)
	{
		fprintf(stderr, "randIni ne peut pas prendre une taille de zeros comme entree !\n");
		exit(-1);
	}
	
	return createNodeStaticArray(start, end);//return the id of the created wrapper
}

void randEnd(unsigned int id)
{
	/* user interface */
	freeStaticArray(id);//remove the wrapper from the list of wrapper
}

int randArray(unsigned int id)
{
	/* return a unique random element from an array include in the wrapper with the given id
	 * or -1 if all the value inside the array have been picked
	*/
	
	if(_list==NULL)
	{
		fprintf(stderr, "Impossible d'executer randArray() sans avoir initialiser l'object\n");
		exit(-1);
	}
	
	container * c = getElem(id);//get the corresponding wrapper
	if(c==NULL || c->size==0)
	{
		return -1;
	}
	
	pArray cursor = c->data;
	pArray p_cursor = NULL;
	unsigned int el = rand()%c->size;
	//get the index of the element to get between 0 and the size of the linked list of values
	
	unsigned int i;
	
	i=0;
	while(i<el)//find the element inside the linked list
	{
		p_cursor = cursor;
		cursor = cursor->next;
		i++;
	}
	
	int val = uIntValue(cursor);//retrieve the value of the element
	
	//remove the current element of the liked list of values
	if(p_cursor==NULL)
	{
		pArray temp = cursor->next;
		freeNodeArray(cursor);
		c->data = temp;
	}else
	{
		free(removeElemArray(p_cursor));//free the element removed from the array
	}
	c->size = c->size -1;//decrease the size of the linkes list of values
	
	return val;
}


/**********************************************************************
 * 
 * 				RANDOM WITH PROBABILITY ARRAY
 * 
 **********************************************************************/

static pArray _listProba = NULL;


typedef struct CumulativeProbabilityHandler * pProbaH;
typedef struct CumulativeProbabilityHandler//wrapper
{
	double * proba;
	double * cumulProba;
	unsigned int size;
} probaH;

void freeProbaHandler(void * p)
{
	 if(p==NULL)
		return;
		
	 pProbaH q = (pProbaH)p;
	 free(q->proba);
	 free(q->cumulProba);
	 free(q);
}

static pProbaH createCumulProbaHandler(double * proba, unsigned int size)
{
	 pProbaH node = (pProbaH)malloc(sizeof(probaH));
	 assert(node);
	 
	 node->proba = proba;
	 node->cumulProba = getCumulativeProba(proba, size);
	 node->size = size;
	 
	 return node;
}

pProbaH getCumulProbaHandler(unsigned int id)
{
	pWrapper p = wrapperGetElem(_listProba, id);
	if(p==NULL || p->data==NULL)
		return NULL;
	
	return ((pProbaH)wrapperGetElem(_listProba, id)->data);
}

unsigned int randCumulProbaIni(double * proba, unsigned int size)
{
	static unsigned int id = 0;
	unsigned int tempId = id;
	id++;
	
	if(proba==NULL)
		return -1;
	
	pProbaH node = createCumulProbaHandler(proba, size);
	wrapperAddToList(&_listProba, wrapperCreateCustomNode(tempId, freeProbaHandler, (void *)node));
	
	return tempId;
}

void randCumulProbaEnd(unsigned int id)
{
	wrapperRemoveFromList(&_listProba, id);
}

void randCumulProbaUpdate(unsigned int id, unsigned int index)
{
	pWrapper p = wrapperGetElem(_listProba, id);
	if(p==NULL || p->data==NULL)
		return ;
		
	pProbaH q = (pProbaH)(p->data);
	updateCumulativeProba(q->cumulProba, q->proba, q->size, index);
}


void randCumulProbaChangeData(unsigned int id, double * proba, unsigned int size)
{
	pWrapper p = wrapperGetElem(_listProba, id);
	if(p==NULL || p->data==NULL)
		return ;
		
	pProbaH q = (pProbaH)(p->data);
	
	if(size!=q->size)
	{
		free(q->cumulProba);
		q->cumulProba = (double *)malloc(sizeof(double)*size);
		assert(q->cumulProba);
	}
	
	q->proba = proba;
	
	updateCumulativeProba(q->cumulProba, q->proba, q->size, 0);
}

int randCumulProba(unsigned int id)
{
	pWrapper p = wrapperGetElem(_listProba, id);
	if(p==NULL || p->data==NULL)
		return -1;
	
	pProbaH q = (pProbaH)(p->data);
	
	double rand_val = randf(0, q->cumulProba[q->size-1]);
	
	return getIndexCumulativeProba(q->cumulProba, q->size, rand_val);
}



/**********************************************************************
 * 
 * 						RANDOM IN SEGMENT
 * 
 **********************************************************************/


int getRandomInSegment(int start, int end)
{
	int m = end-start+1;
	
	if(m==0)
		return start;
	
	return start + rand()%m;
}


/**********************************************************************
 * 
 * 					RANDOM NUMBERS WITH FIXED SUM
 * 
 **********************************************************************/

unsigned int * randomFixedSum(unsigned int * real_sum, unsigned int n, unsigned int sum){
	
	unsigned int * output = (unsigned int *)malloc(sizeof(unsigned int)*n);
	assert(output);
	
	unsigned int i;
	unsigned int count = 0;
	
	for(i=0; i<n; i++){
		output[i] = 1 + rand()%(sum-n+1);
		count+=output[i];
	}
	
	double factor = ((float)sum-n)/((float)count);
	
	*real_sum = 0;
	for(i=0; i<n; i++){
		if(i%2)
		    output[i]= ceil(output[i]*factor)+1;
		else
		    output[i]= floor(output[i]*factor)+1;

		(*real_sum)+=output[i];
	}
	
	return output;
}



/**********************************************************************
 * 
 * 							RANDOM FLOAT
 * 
 **********************************************************************/

double randf(double start, double end)
{
	return start + ((double)rand())/((double)RAND_MAX)*(end-start);
}
