#ifndef _STORAGE_H
#define _STORAGE_H

typedef enum Types {uInt_t, place_t, transition_t, petri_t} types;

/*********************************************************************
 * 							LINKED LIST
 *********************************************************************/
	// * struct * //
typedef struct Array * pArray;
typedef struct Array
{
	types type;//type of the void *
    void * data;
    struct Array * next;
} array;

	// * associated functions * //
pArray createNode(types type, void * data);
pArray add(pArray l, types type, void * data);

unsigned int lengthArray(pArray p);

void initializeArray(pArray a[], unsigned int n);

void freeArray(pArray p);
void freeList(pArray a[], unsigned int n);
 
 
 
/*********************************************************************
 * 				  UNSIGNED INT TYPE (wrapper class)
 *********************************************************************/
	// * struct * //
typedef struct _Int * pInt;
typedef struct _Int
{
	unsigned int data;
} Int;

	// * associated functions * //
pInt uIntCreateNode(unsigned int data);
unsigned int uIntValue(pArray p);
void uIntFree(pInt p);



/*********************************************************************
 * 							Place Type
 *********************************************************************/
	// * struct * //
typedef struct Place * pPlace;
typedef struct Place
{
	unsigned int label;
	int M;
} place;

	// * associated functions * //
pPlace placeCreateNode(unsigned int label);
void placeSetM(pPlace p, int M);
unsigned int placeLabel(pArray p);
int placeM(pArray p);
void placeFree(pPlace p);



/*********************************************************************
 * 							Transition Type
 *********************************************************************/
	// * struct * //
typedef struct Transition * pTransition;
typedef struct Transition
{
	pPlace input; //input place
	pArray output; //output places
	int in_weigth;//input weigth
	int out_weigth; //output weigth
} transition;

	// * associated functions * //
pTransition transitionCreateNode(pPlace input, pArray output);
void transitionSetWeigth(pTransition p, int in_w, int out_w);
pArray transitionOutput(pArray p);
pPlace transitionInput(pArray p);
int transitionInWeigth(pArray p);
int transitionOutWeigth(pArray p);
void transitionFree(pTransition p);



/*********************************************************************
 * 							Petri Type
 *********************************************************************/
	// * struct * //
typedef struct Petri * pPetri;
typedef struct Petri
{
	pPlace node; //place
	pArray input_t; //input transitions of the place
	pArray output_t; //output transitions of the place
} petri;
	
	// * associated functions * //
pPetri petriCreateNode(pPlace node, pArray input, pArray output);
void petriFree(pPetri p);



/*********************************************************************
 * 								GRAPH 
 *********************************************************************/
	// * struct * //
typedef struct Graph
{
	pArray * data;
	unsigned int size;
} graph;

	// * associated functions * //
void graphFree(graph * g);


#endif 
