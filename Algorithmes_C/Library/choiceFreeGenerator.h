#ifndef _CHOICE_FREE_GENERATOR_H
#define _CHOICE_FREE_GENERATOR_H

#include <glpk.h> 
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>


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

void freeArray(pArray p);//free all elements in the array


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

void freeArray2(pArray2 p);


/*********************************************************************
 * 							PETRI NET
 *********************************************************************/

#define PETRI_PLACE_TYPE 1
#define PETRI_TRANSITION_TYPE 0
#define PETRI_PT_LINK 2
#define PETRI_TP_LINK 3

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

pPetriLink petriNodeGetLinkFromArrayNode(pArray node);//from an element of a linked list of a node retrieve the corresponding link


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
 * 							GENERATOR
 *********************************************************************/
 
//Initialization of SDF
unsigned int * weightsComputation(unsigned int * real_vect_norm,
                                  unsigned int nb_transition, unsigned int repetition_vect_norm);//create random repetition vector (where the gcd of the array is 1)

//generate random Choice-Free
/* The transformation from SDF to Choice-Free (used to generate a random Choice-Free) leave empty memory space inside the petri structure
   If "cleanExtraMemSpace" is set to 0, then these empty space will remain in the petri net after the transformation (somme value of
   the fixed size arrays inside the petri structure will be set to NULL)
   Else the petri net will be resized, which can take extra time
*/
pPetri generateRandomChoiceFree(unsigned int * real_vect_norm, 
								unsigned int nb_transition, double density, unsigned int repetition_vect_norm, int SDF, int cleanExtraMemSpace);//Free-choice from its repetition vector norm
pPetri generateChoiceFreeWithVector(unsigned int nb_transition, double density, unsigned int * repetition_vect, int SDF, int cleanExtraMemSpace);//Free-choice from its repetition vector



/*********************************************************************
 * 							DIPSLAY
 *********************************************************************/

void petriToPnmlDisplay(pPetri net, char * network_name);
void petriToPnmlFile(pPetri net, char * network_name, char* filename);

void displayPetriNet(pPetri net);
void displayPetriNode(pPetriNode p, unsigned int index);
void displayPetriLink(pPetriLink p);
void displayPetriElem(pPetriElem p);


#endif 
