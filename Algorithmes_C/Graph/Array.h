#ifndef _STORAGE_H
#define _STORAGE_H

typedef enum Types {uInt_t, place_t, transition_t, petri_t, wrapper_t, array_t, list_t} types;

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

void initializeList(void * a[], unsigned int n);

void freeArray(pArray p);
void freeList(pArray a[], unsigned int n);
 
void removeElemArray(pArray previous_elem);
void freeNodeArray(pArray node);
void freeType(types type, void * pData);
 
 
/*********************************************************************
 * 								LIST 
 *********************************************************************/
	// * struct * //
typedef struct List * pList;
typedef struct List
{
	pArray * data;
	unsigned int size;
} list;

	// * associated functions * //
pList listCreate(pArray * p, unsigned int size);
void listFree(list * g);


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

void initializeIntArray(int list[], unsigned int n, int value);

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
	int * places;//1D array
	unsigned int num_pl;//number of places
	int * trans;//2D array
	unsigned int num_tr;//number of transitions
} petri;
	
	// * associated functions * //
pPetri petriCreateNode(int * places, unsigned int num_pl, int * trans, unsigned int num_tr);
void petriFree(pPetri p);


/*********************************************************************
 * 								WRAPPER 
 *********************************************************************/
	// * struct * //
typedef struct Wrapper * pWrapper;
typedef struct Wrapper
{
	types type;
	unsigned int id;
	void * data;
} wrapper;

	// * associated functions * //
void wrapperFree(pWrapper p);
pArray wrapperAddToList(pArray  w_list, unsigned int id, types type, void * data);
int wrapperGetId(pArray p);
pWrapper wrapperGetElem(pArray w_list, unsigned int id);
pArray wrapperRemoveFromList(pArray w_list, unsigned int id);
pArray wrapperFreeList(pArray w_list);

#endif 
