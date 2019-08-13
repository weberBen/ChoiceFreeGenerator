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

/* The following substructure are not meant to be stand alone one 
   Memory management are made within the global petri structure and not individually 
*/

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

/*
	Each element of the petri network will be saved under this structure.
	It contains :
		- the type of element (place or transition) for which the value is defined
	by the directives "PETRI_PLACE_TYPE" and "PETRI_TRANSITION_TYPE"
		- the label of the element which here correspond to the index of the element inside a fixed size array (for exemple, the
	label 0 represents the position of that element inside an fixed size array as the element 0)
		- Exclusively for a place its initial marking
	*/
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
void petriElemFree(pPetriElem p);//can be used as stand alone


/* A link represent an oriented edge inside the petri net between two elements, with a weight
   Both input and output elements are saved as pointers to the actual element (to update all the references
   of an element once, if a value need to be changed)	
*/
typedef struct PetriLink * pPetriLink;
typedef struct PetriLink
{
	pPetriElem input;
	pPetriElem output;
	unsigned int weight;
} petriLink;

pPetriLink petriLinkCreate(pPetriElem input, pPetriElem output, int weight);
void petriLinkFree(pPetriLink p);//cannot be used as stand alone
/* Because the reference of an element will be store in a fixed size array and in multiple links,
   release memory from a link must not free the memory of the elements inside 
   (for example a link between two node can be removed without having to remove the nodes)*/


/* A node can be a place or a transition inside the petri net for which all the input and output links from and to that
   element are included. Thus, retrieve a relation from a given node is easier.
   A node contains :
		- the number of input elements to that one
		- a linked list of the references of the links in which the current element is contains as an output
		- the number of output elements from that one
		- a linked list of the references of the links in which the current element is contains as an input
	
	The input links represent the links where the current element is an output (because the other element will
	be an input inside that link which connect from the input element to the current one)
	For example the link elem1 ----> elem2 will be store inside the linked list of input links for the node of elem2
	and inside the output links for the node of elem1


	All the links will be store inside a double linked list (from an element inside the list the previpous and next elements 
	can be retrieve). It's allow to store the reference of a link inside the linked list of a node and if needed remove that link
	from the double linked list only given that reference to the link
 */

typedef struct PetriNode * pPetriNode;
typedef struct PetriNode
{
	unsigned int nb_inputs;
	pArray input_links;//linked list of petri links (inputs of the node, then the node is the output inside the link)
	unsigned int nb_outputs;
	pArray output_links;//linked list of petri links (ouputs of the node, then the node is the input inside the link)
} petriNode;

pPetriNode petriNodeCreate();
void petriNodeAddInput(pPetriNode node, pArray2 plink_in_array);//take the reference of a link inside the double linked list of links
void petriNodeAddOutput(pPetriNode node, pArray2 plink_in_array);//take the reference of a link inside the double linked list of links
void petriNodeRemoveInput(pPetriNode node, pArray2 plink_in_array);//take the reference of a link inside the double linked list of links
void petriNodeRemoveOutput(pPetriNode node, pArray2 plink_in_array);//take the reference of a link inside the double linked list of links
pPetriLink petriNodeGetLinkFromArrayNode(pArray node);//from an element of a linked list of a node retrieve the corresponding link
void petriNodeFree(pPetriNode p);//can not be used as stand alone
/* The function free the linked lists inside the node but not the elements (the references to the links) strore inside it
   In other words, after freeing a node all the links include in it will remain in memory (in the double linked list of links)*/

/*
	The petri strcuture is quite heavy in memory because links are store multiple time (as references) to make the manipulation
	of each element easier 
	It contains :
		- the number of places
		- a fixed size array of all the places as node (with its input and output links), initially set to NULL
		- the number of transitions
		- a fixed size array of all the transitions as node (with its input and output links), initially set to NULL
		- the number of links
		- a double linked list containing all the links
		- a fixed size array of all the places (as elements), initially set to NULL
		- a fixed size array of all the transitions (as elements), initially set to NULL

	The number of places is the size of the fixed size arrays of places as node and as element (the same for the number of transitions)
	When a node, or an element as not been set (or has been removed) its value at the index of the element (set as its label) is set to NULL
	
	A node cannot be removed inside the petri net : the removal of an element (a place or a transition) implies the removal 
	of the corresponding node and its links

	The advantage of that structure is to access all the element rapidly. For example if the user want to access all the links of the 
	transitions i, it can access its corresponding node inside the fixed size array of node at the index i and loop through all its
	links thanks to the linked list of input and output links
	A change of attribute (for example a label or an initial marking for an element, or a weight for a links) impacts all the node
	because only the reference are store

	The double linked list of links, fixed size arrays of place and transitions, all to remove it one to release the memory
*/

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
} petri;
	
	// * associated functions * //

pPetri petriCreate(unsigned int nb_pl, unsigned int nb_tr);
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
/* When an element (place or transition) is removed from the petri net the fixed size arrays are not resized, only the value
at the corresponding index are set to NULL (to not decrease the speed of the transformation.
The following functions resize these fixed size array and make the needed change (as for the label of an element which need
to match the new index of the resized fixed size array)*/
void petriClearPlaces(pPetri net);
void petriClearTransitions(pPetri net);

void petriFree(pPetri p);//only that function must be used to free a petri net

/*********************************************************************
 * 						WRAPPER (used fo pyhton)
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
