#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Array.h"

 

/*********************************************************************
 * 
 * 					
 * 						SIMPLE LINKED LIST FUNCTIONS
 * 
 * 
 *********************************************************************/

pArray _createNode(types type, void (* freeFunction)(void *pData), void * data)
{
	if(type==custom_t && freeFunction==NULL)
	{
		fprintf(stderr, "Creation d'un noeud personalise mais fonction de liberation de la memoire donne invalide (NULL)\n");
		exit(-1);
	}
    pArray node = (pArray)malloc(sizeof(array));
    assert(node);

	node->type = type;
	node->freeFunction = freeFunction;
    node->data = data;
    node->next = NULL;

    return node;
}


pArray createNode(types type, void * data)
{
    return _createNode(type, NULL, data);
}


pArray createCustomNode(void (* freeFunction)(void *pData), void * data)
{
	return _createNode(custom_t, freeFunction, data);
}


pArray _add(pArray l, types type, void * data, void (* freeFunction)(void *pData))
{
    pArray node;
	if(freeFunction==NULL)
		node = createNode(type, data);
	else
		node = createCustomNode(freeFunction, data);
    node->next = l;

    return node;
}


pArray add(pArray l, types type, void * data)
{
    return _add(l, type, data, NULL);
}

pArray addCustom(pArray l, void * data, void (* freeFunction)(void *pData))
{
    return _add(l, custom_t, data, freeFunction);
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

int referenceComparaison(void * data, pArray compareTo)
{
	if(data==NULL || compareTo==NULL)
		return 0;//false
	
	pArray p = (pArray)data;

	return (p==compareTo);//pointer reference comporaison
}

pArray getArrayElement(pArray p, void * data, int (* booleanFunction)(void * ref, pArray compareTo))
{
	//return the corresponding element in the array using a custom comparaison function from a pointer to the desire data to find
	
	while(p!=NULL)
	{
		if(booleanFunction(data, p))//check if p contain the needed data
		{
			return p;//searched element
		}
		p = p->next;
	}
	
	return NULL;
}

void freeType(types type, void (* freeFunction)(void * pData), void * pData)
{
	switch(type)
	{
		case uInt_t:
			uIntFree((pInt)(pData));
			break;
		case directedGraph_t:
			directedGraphFree((pDirectedGraph)(pData));
			break;
		case petriElem_t:
			petriElemFree((pPetriElem)(pData));
			break;
		case petriLink_t:
			petriLinkFree((pPetriLink)(pData));
			break;
		case petriNode_t:
			petriNodeFree((pPetriNode)(pData));
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
		case custom_t:
			if(freeFunction==NULL)
			{
				fprintf(stderr, "Creation d'un noeud personalise mais fonction free passe en argumument NULL\n");
				exit(-1);
			}
			freeFunction(pData);
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
		
	freeType(node->type, node->freeFunction, node->data);
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

pArray removeElemArray(pArray previous_elem)
{
	/* From the previous node in the list remove the next node
	   and return that node, else return NULL */
	if(previous_elem==NULL)
		return NULL;
	
	pArray cursor = previous_elem->next;//element to remove
	previous_elem->next = cursor->next;
	cursor->next=NULL;
	
	return cursor;
}

pArray removeFirstElemArray(pArray * pp)
{
	/* From an array remove the first element and return that element */
	pArray p = *pp;

	if(p==NULL)
		return NULL;
	
	pArray cursor = p;
	*pp = p->next;
	cursor->next = NULL;

	return cursor;
}

pArray removeCustomElemArray(pArray * pp, int (* booleanFunction)(void * data, pArray input_to_compare), void * data)
{
	/* From an array compare each element through a custom function to a reference pointer and if
	   a node match that reference, then remove that node from the array and return it.
	   If no occurence of the reference has been founded return NULL
	*/
	pArray cursor = *pp;
	pArray prev = NULL;
	pArray temp;

	while(cursor!=NULL)
	{
		if(booleanFunction(data, cursor))
		{
			if(prev==NULL)//first element of the list
				temp = removeFirstElemArray(pp);
			else
				temp = removeElemArray(prev);
			
			return temp;
		}
		prev = cursor;
		cursor = cursor->next;
	}

	return NULL;
}

/*********************************************************************
 * 
 * 					
 * 						DOUBLE LINKED LIST FUNCTIONS
 * 
 * 
 *********************************************************************/

pArray2 _createNode2(types type, void (* freeFunction)(void *pData), void * data)
{
	if(type==custom_t && freeFunction==NULL)
	{
		fprintf(stderr, "Creation d'un noeud personalise mais fonction de liberation de la memoire donne invalide (NULL)\n");
		exit(-1);
	}
    pArray2 node = (pArray2)malloc(sizeof(array2));
    assert(node);

	node->type = type;
	node->freeFunction = freeFunction;
    node->data = data;
    node->next = NULL;
	node->prev = NULL;

    return node;
}

pArray2 createNode2(types type, void * data)
{
    return _createNode2(type, NULL, data);
}


pArray2 createCustomNode2(void (* freeFunction)(void *pData), void * data)
{
	return _createNode2(custom_t, freeFunction, data);
}


pArray2 _add2(pArray2 l, types type, void * data, void (* freeFunction)(void *pData))
{
    pArray2 node;
	if(freeFunction==NULL)
		node = createNode2(type, data);
	else
		node = createCustomNode2(freeFunction, data);
	
	if(l!=NULL)
		l->prev = node;
    node->next = l;
	node->prev = NULL;
	
    return node;
}


pArray2 add2(pArray2 l, types type, void * data)
{
    return _add2(l, type, data, NULL);
}

pArray2 addCustom2(pArray2 l, void * data, void (* freeFunction)(void *pData))
{
    return _add2(l, custom_t, data, freeFunction);
}


pArray2 removeElemArray2(pArray2 * _array, pArray2 elem)
{
	/* From the previous node in the list remove the next node
	   and return that node, else return NULL */
	if(elem==NULL || _array==NULL)
		return NULL;
	
	pArray2 p = *_array;
	pArray2 node;

	if(elem->prev==NULL)//first element
	{
		node = p;
		*_array = p->next;
		(p->next)->prev = NULL;

	}else
	{
		node = elem;
		(node->prev)->next = node->next;
		if(node->next!=NULL)//not last element
			(node->next)->prev = node->prev;
	}

	node->prev = NULL;
	node->next = NULL;

	return node;
}


void freeNodeArray2(pArray2 node)
{
	if(node==NULL)
		return;
		
	freeType(node->type, node->freeFunction, node->data);
	free(node);
}

void freeArray2(pArray2 p)
{
	pArray2 cursor = p;
	
	while(cursor!=NULL)
	{
		p = cursor->next;
		freeNodeArray2(cursor);
		cursor = p;
	}
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



int comparaison(void * ref, pArray compareTo)
{
	
	if(compareTo==NULL || compareTo->type!=uInt_t)
		return 0;//false
		
	if(ref==NULL && compareTo->data==NULL)
		return 1;//true
	else if(ref==NULL || compareTo->data==NULL)
		return 0;//false
	
	int * p1 = (int *)ref;
	pInt p2 = (pInt)(compareTo->data);
	
	return *p1==p2->data;
}


/*********************************************************************
 * 
 * 					
 * 						ORIENTED GRAPH FUNCTIONS
 * 
 * 
 *********************************************************************/

pDirectedGraph directedGraphCreate(unsigned int nb_nodes)
{
	pArray * links_list = (pArray *)malloc(sizeof(array)*nb_nodes);
	assert(links_list);
	initializeList((void *)links_list, nb_nodes);

	pDirectedGraph output = (pDirectedGraph)malloc(sizeof(directedGraph));
	assert(output);
	output->links_list = links_list;
	output->nb_nodes = nb_nodes;
	output->nb_edges = 0;

	return output;
}

void directedGraphAddLink(pDirectedGraph graph, unsigned int src_node, unsigned int dest_node)
{
	pArray * links_list = graph->links_list;
	if(links_list==NULL)
	{
		fprintf(stderr, "Tentative d'ajout d'un lien entre deux noeuds dans un graph vide\n");
		return;
	}
	links_list[src_node] = add(links_list[src_node], uInt_t, uIntCreateNode(dest_node));
	graph->nb_edges = graph->nb_edges + 1;
}


void directedGraphFree(pDirectedGraph p)
{
	if(p!=NULL)
	{
		freeList(p->links_list, p->nb_nodes);
		free(p);
	}
}
 
 
 /*********************************************************************
 * 
 * 					
 * 						PETRI FUNCTIONS
 * 
 * 
 *********************************************************************/

		 /*********************************************************************				
		* 						PETRI ELEM FUNCTIONS
		*********************************************************************/


 pPetriElem petriElemCreate(int type, unsigned int label, int val)
 {
	 pPetriElem output = (pPetriElem)malloc(sizeof(petriElem));
	 assert(output);
	 
	 output->type = type;
	 output->label = label;
	 output->val = val;

	 return output;
 }


 pPetriElem petriElemCreatePlace(unsigned int label, int val)
 {
	 return petriElemCreate(PETRI_PLACE_TYPE, label, val);
 }

 pPetriElem petriElemCreateTransition(unsigned int label)
 {
	 return petriElemCreate(PETRI_TRANSITION_TYPE, label, -1);
 }

 void petriElemFree(pPetriElem p)
 {
	 if(p!=NULL)
	 	free(p);
 }


		 /*********************************************************************				
		* 						PETRI LINK FUNCTIONS
		*********************************************************************/


 pPetriLink petriLinkCreate(pPetriElem input, pPetriElem output, int weight)
 {
	 pPetriLink _output = (pPetriLink)malloc(sizeof(petriLink));
	 assert(_output);
	 _output->input = input;
	 _output->output = output;
	 _output->weight = weight;

	 return _output;
 }

 void petriLinkFree(pPetriLink p)
 {
	 if(p==NULL)
	 	return;

	petriElemFree(p->input);
	petriElemFree(p->output);
	free(p);

 }

 void petriLinkFreeFunctionArray(void * pData)
 {
	 if(pData==NULL)
	 	return;
	
	 pPetriLink p = (pPetriLink)pData;

	 free(p);
	 //do not free input and output element
 }


		 /*********************************************************************				
		* 						PETRI NODE FUNCTIONS
		*********************************************************************/

static void petriNodeFreeFunctionArray(void * pData);

int petriLinkComparaison(void * data, pArray input_to_compare)
{
	if(data==NULL || input_to_compare==NULL)
		return 0;//false
	if(input_to_compare->data==NULL)
		return 0;
	
	pArray2 ref = (pArray2)data;
	pArray2 compareTo = (pArray2)(input_to_compare->data);

	return (ref==compareTo);//pointer reference comporaison
}


pPetriNode petriNodeCreate()
{
	pPetriNode output = (pPetriNode)malloc(sizeof(petriNode));
	assert(output);

	output->nb_inputs = 0;
	output->input_links = NULL;
	output->nb_outputs = 0;
	output->output_links = NULL;

	return output;
}

void petriNodeAddInput(pPetriNode node, pArray2 plink_in_array)
{
	if(node==NULL)
		return;
	
	node->input_links = addCustom(node->input_links, plink_in_array, petriNodeFreeFunctionArray);
	node->nb_inputs = node->nb_inputs +1;
}


void petriNodeAddOutput(pPetriNode node, pArray2 plink_in_array)
{
	if(node==NULL)
		return;
	
	node->output_links = addCustom(node->output_links, plink_in_array, petriNodeFreeFunctionArray);
	node->nb_outputs = node->nb_outputs +1;
}

void petriNodeRemoveInput(pPetriNode node, pArray2 plink_in_array)
{
	pArray tmp = removeCustomElemArray(&(node->input_links), petriLinkComparaison, plink_in_array);
	if(tmp!=NULL)//occurence found
		node->nb_inputs--;
	free(tmp);//does not free the data inside
}

void petriNodeRemoveOutput(pPetriNode node, pArray2 plink_in_array)
{
	pArray tmp = removeCustomElemArray(&(node->output_links), petriLinkComparaison, plink_in_array);
	if(tmp!=NULL)//occurence found
		node->nb_outputs--;
	free(tmp);//does not free the data inside
}

pPetriLink petriNodeGetLinkFromArrayNode(pArray node)
{
	if(node==NULL)
		return NULL;
	
	pArray2 node_array = (pArray2)(node->data);
	if(node_array==NULL)
		return NULL;
	
	return (pPetriLink)(node_array->data);
}

void petriNodeFree(pPetriNode p)
{
	if(p==NULL)
		return;
	
	freeArray(p->input_links);
	freeArray(p->output_links);
	free(p);
}


static void petriNodeFreeFunctionArray(void * pData)
{
	//do nothing
}


 		/*********************************************************************				
		* 						PETRI NET FUNCTIONS
		*********************************************************************/

pPetri petriCreate(unsigned int nb_pl, unsigned int nb_tr)
{
	pPetri output = (pPetri)malloc(sizeof(petri));
	assert(output);
	
	output->nb_pl = nb_pl;
	output->nb_tr = nb_tr;

	output->places = (pPetriNode *)malloc(sizeof(pPetriNode)*nb_pl);
	assert(output->places);
	output->transitions = (pPetriNode *)malloc(sizeof(pPetriNode)*nb_tr);
	assert(output->transitions);

	output->nb_links = 0;
	output->links = NULL;

	output->pl_elems = (pPetriElem *)malloc(sizeof(pPetriElem)*nb_pl);
	assert(output->pl_elems);
	output->tr_elems = (pPetriElem *)malloc(sizeof(pPetriElem)*nb_tr);
	assert(output->tr_elems);

	//initialization
	int i;
	int limit_inf = min(nb_pl, nb_tr);
	int limit_sup = max(nb_pl, nb_tr);

	for(i=0; i<limit_inf; i++)
	{
		output->places[i] = NULL;
		output->transitions[i] = NULL;
		output->pl_elems[i] = NULL;
		output->tr_elems[i] = NULL;
	}

	pPetriNode * tmp_node;
	pPetriElem * tmp_elem;

	if(nb_pl==limit_inf)
	{
		tmp_node = output->transitions;
		tmp_elem = output->tr_elems;
	}else
	{
		tmp_node = output->places;
		tmp_elem = output->pl_elems;
	}

	for(i=limit_inf; i<limit_sup; i++)
	{
		tmp_node[i] = NULL;
		tmp_elem[i] = NULL;
	}


	return output;
}

void petriAddPlace(pPetri net, unsigned int index, unsigned int initial_marking)
{
	if(index>=net->nb_pl || net->pl_elems[index]!=NULL)
	{
		fprintf(stderr, "Ajout de place impossible dans le reseau de Petri\n");
		return ;
	}

	net->pl_elems[index] = petriElemCreatePlace(index, initial_marking);
}


void petriAddTransition(pPetri net, unsigned int index)
{
	if(index>=net->nb_tr || net->tr_elems[index]!=NULL)
	{
		fprintf(stderr, "Ajout de la transition impossible dans le reseau de Petri\n");
		return ;
	}

	net->tr_elems[index] = petriElemCreateTransition(index);
}

void petriAddlink(pPetri net, int input_type, unsigned int input, int output_type, unsigned int output, int weight)
{
	if(input_type==output_type)
	{
		fprintf(stderr, "Impossible d'ajouter un lien entre deux objets du même type (Pl-Pl ou Tr-Tr) dans le reseau de Petri\n");
		return;
	}

	//create link element
	pPetriElem * inputs_elem_array, * outputs_elem_array;
	unsigned int size_inputs_array, size_outputs_array;
	if(input_type==PETRI_PLACE_TYPE)
	{
		inputs_elem_array = net->pl_elems;
		size_inputs_array = net->nb_pl;
		outputs_elem_array = net->tr_elems;
		size_outputs_array = net->nb_tr;
	}else
	{
		inputs_elem_array = net->tr_elems;
		size_inputs_array = net->nb_tr;
		outputs_elem_array = net->pl_elems;
		size_outputs_array = net->nb_pl;
	}

	if(input>=size_inputs_array || output>=size_outputs_array)
	{
		fprintf(stderr,"Element hors index selectionne pour la creation d'un lien dans le reseau de Petri\n");
		return;
	}

	pPetriElem input_elem = inputs_elem_array[input];
	pPetriElem output_elem = outputs_elem_array[output];

	if(input_elem==NULL || output_elem==NULL)
	{
		fprintf(stderr,"Impossible d'ajouter un lien entre deux objets qui n'existe pas dans le reseau de pPetri\n");
		return;
	}


	pPetriLink link = petriLinkCreate(input_elem, output_elem, weight);
	net->links = addCustom2(net->links, link, petriLinkFreeFunctionArray);
	pArray2 plink_in_array = net->links;
	net->nb_links++;

	//add corresponding nodes to the network
	pPetriNode * inputs_node_array, * outputs_node_array;
	if(input_type==PETRI_PLACE_TYPE)
	{
		inputs_node_array = net->places;
		outputs_node_array = net->transitions;
	}else
	{
		inputs_node_array = net->transitions;
		outputs_node_array = net->places;
	}

	//create node if necessary
	if(inputs_node_array[input]==NULL)
	{
		inputs_node_array[input] = petriNodeCreate();
	}
	if(outputs_node_array[output]==NULL)
	{
		outputs_node_array[output] = petriNodeCreate();
	}
	petriNodeAddOutput(inputs_node_array[input], plink_in_array);
	petriNodeAddInput(outputs_node_array[output], plink_in_array);
}


void petriRemovePlace(pPetri net, unsigned int index)
{
	if(index>=net->nb_pl)
	{
		fprintf(stderr,"Suppression d'un element hors index dans le reseau de Petri\n");
		return;
	}

	if(net->places[index]==NULL)
		return;

	pPetriNode node = net->places[index];
	pArray links_array = node->input_links;
	pPetriLink link;

	while(links_array)
	{
		link = petriNodeGetLinkFromArrayNode(links_array);

		freeNodeArray(links_array);

		
		links_array = links_array->next;
	}
}
/*typedef struct Petri * pPetri;
typedef struct Petri
{
	unsigned int nb_pl;
	petriNode * places;
	unsigned int nb_tr;
	petriNode * transitions;
	unsigned int nb_links;
	pArray2 links;//linked list of petri links
	petriElem * pl_elems;
	petriElem * tr_elems;
} petri;*/



void petriFree(pPetri p)
{
	if(p==NULL)
		return;

	int i;
	int limit_inf = min(p->nb_pl, p->nb_tr);
	int limit_sup = max(p->nb_pl, p->nb_tr);

	for(i=0; i<limit_inf; i++)
	{
		petriNodeFree(p->places[i]);
		petriNodeFree(p->transitions[i]);
		petriElemFree(p->pl_elems[i]);
		petriElemFree(p->tr_elems[i]);
	}

	pPetriNode * tmp_node;
	pPetriElem * tmp_elem;

	if(p->nb_pl==limit_inf)
	{
		tmp_node = p->transitions;
		tmp_elem = p->tr_elems;
	}else
	{
		tmp_node = p->places;
		tmp_elem = p->pl_elems;
	}

	for(i=limit_inf; i<limit_sup; i++)
	{
		petriNodeFree(tmp_node[i]);
		petriElemFree(tmp_elem[i]);
	}

	free(p->places);
	free(p->transitions);
	free(p->pl_elems);
	free(p->tr_elems);

	freeArray2(p->links);

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
	
	freeType(p->type, p->freeFunction, p->data);
	free(p);
}

pWrapper _wrapperCreateNode(unsigned int id, types type, void (* freeFunction)(void * pData), void * data)
{
	if(type==custom_t && freeFunction==NULL)
	{
		fprintf(stderr, "Creation d'un noeud personalise mais fonction de liberation de la memoire donne invalide (NULL)\n");
		exit(-1);
	}
	
	pWrapper c = (pWrapper)malloc(sizeof(wrapper));
	assert(c);
	
	c->type = type;
	c->freeFunction = freeFunction;
	c->id = id;
	c->data = data;
	
	return c;
}


pWrapper wrapperCreateNode(unsigned int id, types type, void * data)
{
	return _wrapperCreateNode(id, type, NULL, data);
}

pWrapper wrapperCreateCustomNode(unsigned int id, void (* freeFunction)(void * pData), void * data)
{
	return _wrapperCreateNode(id, custom_t, freeFunction, data);
}


void  wrapperAddToList(pArray * pp_list, pWrapper c)
{
	pArray w_list = * pp_list;
	w_list = add(w_list, wrapper_t, c);
	
	* pp_list = w_list;
	return ;
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

void  wrapperRemoveFromList(pArray * pp_list, unsigned int id)
{
	pArray w_list = * pp_list;
	if(w_list==NULL)
		return ;
	
	pArray cursor = w_list;
	pArray p_cursor = NULL;
	int c_id;
	
	while(cursor && (c_id=wrapperGetId(cursor))!=id)//get the wrapper
	{
		p_cursor = cursor;//previous element
		cursor = cursor->next;
	}
	
	if(c_id==-1 || cursor==NULL)
		return;
	
	
	if(p_cursor==NULL)//first element of the list of wrapper
	{
		cursor = w_list->next;
		freeNodeArray(w_list);
		w_list = cursor;
		
		* pp_list = w_list;
		return ;
	}
	
	p_cursor->next = cursor->next;
	freeNodeArray(cursor);//free the wrapper node
	
	* pp_list = w_list;
	return ;
}

pArray wrapperFreeList(pArray w_list)
{
	freeArray(w_list);
	return NULL;
}
