#ifndef _STORAGE_H
#define _STORAGE_H

#include "Tools.h"

typedef enum Types {custom_t=-1, uInt_t, petri_t, petriElem_t, petriNode_t, petriLink_t, wrapper_t, array_t, list_t, fixedSizeList_t, directedGraph_t} types;

/*********************************************************************
 * 							SIMPLE LINKED LIST
 *********************************************************************/
	// * struct * //
typedef struct Array * pArray;//one side array (can only get a node next to another)
typedef struct Array
{
	types type;//type of void * pointer
	void (*freeFunction)(void * pData);//in case of custom type the corresponding custum function for the void * poiter
    void * data;
    struct Array * next;
} array;


	// * associated functions * //
//Array
pArray createNode(types type, void * data);
pArray createCustomNode(void (* freeFunction)(void *pData), void * data);
pArray add(pArray l, types type, void * data);
pArray addCustom(pArray l, void * data, void (* freeFunction)(void *pData));

unsigned int lengthArray(pArray p);

void freeArray(pArray p);
void freeList(pArray a[], unsigned int n);
void freeNodeArray(pArray node);
 
pArray removeElemArray(pArray previous_elem);
pArray removeFirstElemArray(pArray * pp);
pArray removeCustomElemArray(pArray * pp, int (* booleanFunction)(void * data, pArray input_to_compare), void * data);
void freeType(types type, void (* freeFunction)(void * pData), void * pData);
pArray getArrayElement(pArray p, void * data, int (* booleanFunction)(void * ref, pArray input_to_compare));


/*********************************************************************
 * 							DOUBLE LINKED LIST
 *********************************************************************/
	// * struct * //
typedef struct Array2 * pArray2;//two side array (can only get thes previous and next node from a given node, no loop at the end)
typedef struct Array2
{
	types type;//type of void * pointer
	void (*freeFunction)(void * pData);//in case of custom type the corresponding custum function for the void * poiter
    void * data;
    struct Array2 * next;
	struct Array2 * prev;
} array2;

	// * associated functions * //
pArray2 createNode2(types type, void * data);
pArray2 createCustomNode2(void (* freeFunction)(void *pData), void * data);
pArray2 add2(pArray2 l, types type, void * data);
pArray2 addCustom2(pArray2 l, void * data, void (* freeFunction)(void *pData));

void freeArray2(pArray2 p);
void freeNodeArray2(pArray2 node);

pArray2 removeElemArray2(pArray2 * _array, pArray2 elem);


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
 * 						 FIXED SIZE LIST 
 *********************************************************************/
	// * struct * //
typedef struct FixedSizeList * pFixedSizeList;
typedef struct FixedSizeList
{
	pArray data;
	unsigned int size;
} fixedSizeList;
//allow to gather an array (with its type and its free function through the node array pArray) and its size

	// * associated functions * //
pFixedSizeList fixedSizeListCreate(pArray p, unsigned int size);
void * fixedSizeListGetData(pFixedSizeList p);
void fixedSizeListFree(pFixedSizeList p);


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
int comparaison(void * ref, pArray compareTo);


/*********************************************************************
 * 							DIRECTED GRAPH
 *********************************************************************/
typedef struct _directedGraph * pDirectedGraph;
typedef struct _directedGraph
{
	pArray * links_list;
	unsigned int nb_nodes;
	unsigned int nb_edges;
} directedGraph;


pDirectedGraph directedGraphCreate(unsigned int nb_nodes);
void directedGraphAddLink(pDirectedGraph graph, unsigned int src_node, unsigned int dest_node);
void directedGraphFree(pDirectedGraph p);


/*********************************************************************
 * 							Petri Type
 *********************************************************************/
	// * struct * //

#define PETRI_PLACE_TYPE 1
#define PETRI_TRANSITION_TYPE 0
#define PETRI_PT_LINK 2
#define PETRI_TP_LINK 3

#define petri_type_reverse(type) \
({ __typeof__ (type) _type = (type); \
(_type==PETRI_PLACE_TYPE) ? PETRI_TRANSITION_TYPE : PETRI_PLACE_TYPE; })


#define petri_input_type_link(link_type) \
({ __typeof__ (link_type) _link_type = (link_type); \
(_link_type==PETRI_PT_LINK) ? PETRI_PLACE_TYPE : PETRI_TRANSITION_TYPE; })


typedef struct PetriElem * pPetriElem;
typedef struct PetriElem
{
	int type;
	unsigned int label;
	int val;//for a place it will be use to store the initial marking
} petriElem;

pPetriElem petriElemCreate(int type, unsigned int label, int val);
pPetriElem petriElemCreatePlace(unsigned int label, int val);
pPetriElem petriElemCreateTransition(unsigned int label);
void petriElemFree(pPetriElem p);


typedef struct PetriLink * pPetriLink;
typedef struct PetriLink
{
	pPetriElem input;
	pPetriElem output;
	unsigned int weight;
} petriLink;

pPetriLink petriLinkCreate(pPetriElem input, pPetriElem output, int weight);
void petriLinkFree(pPetriLink p);

typedef struct PetriNode * pPetriNode;
typedef struct PetriNode
{
	unsigned int nb_inputs;
	pArray input_links;//linked list of petri links (inputs of the node, then the node is the output inside the link)
	unsigned int nb_outputs;
	pArray output_links;//linked list of petri links (ouputs of the node, then the node is the input inside the link)
} petriNode;

pPetriNode petriNodeCreate();
void petriNodeAddInput(pPetriNode node, pArray2 plink_in_array);
void petriNodeAddOutput(pPetriNode node, pArray2 plink_in_array);
void petriNodeRemoveInput(pPetriNode node, pArray2 plink_in_array);
void petriNodeRemoveOutput(pPetriNode node, pArray2 plink_in_array);
pPetriLink petriNodeGetLinkFromArrayNode(pArray node);
void petriNodeFree(pPetriNode p);

typedef struct Petri * pPetri;
typedef struct Petri
{
	unsigned int nb_pl;
	pPetriNode * places;
	unsigned int nb_tr;
	pPetriNode * transitions;
	unsigned int nb_links;
	pArray2 links;//linked list of petri links
	pPetriElem * pl_elems;
	pPetriElem * tr_elems;
	unsigned int * repetition_vector;
} petri;
	
	// * associated functions * //

pPetri petriCreate(unsigned int nb_pl, unsigned int nb_tr);
void petriSetRepetitionVect(pPetri net, unsigned int * vect);
void petriAddPlace(pPetri net, unsigned int index, unsigned int initial_marking);
void petriAddTransition(pPetri net, unsigned int index);
void petriAddlink(pPetri net, int link_type, unsigned int input, unsigned int output, int weight);
void petriRemovePlace(pPetri net, unsigned int index);
void petriRemoveTransition(pPetri net, unsigned int index);
pPetriLink petriGetLink(pPetri net, int link_type, unsigned int input, unsigned int output);
int petriGetWeightLink(pPetri net, int link_type, unsigned int input, unsigned int output);
pPetriElem petriGetPlace(pPetri net, unsigned int index);
pPetriElem petriGetTransition(pPetri net, unsigned int index);
int petriGetInitialMarking(pPetri net, unsigned int index);

void petriFree(pPetri p);

/*********************************************************************
 * 								WRAPPER 
 *********************************************************************/
	// * struct * //
typedef struct Wrapper * pWrapper;
typedef struct Wrapper
{
	types type;
	void (*freeFunction)(void * pData);
	unsigned int id;
	void * data;
} wrapper;

	// * associated functions * //
pWrapper wrapperCreateNode(unsigned int id, types type, void * data);
pWrapper wrapperCreateCustomNode(unsigned int id, void (* freeFunction)(void * pData), void * data);
void wrapperFree(pWrapper p);
void  wrapperAddToList(pArray * pp_list, pWrapper c);
int wrapperGetId(pArray p);
pWrapper wrapperGetElem(pArray w_list, unsigned int id);
void  wrapperRemoveFromList(pArray * pp_list, unsigned int id);
pArray wrapperFreeList(pArray w_list);

#endif 
