#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "GraphGenerator.h"
#include "Tools.h"
#include "Rand.h"
#include "Display.h"

#include <sys/types.h>
#include <unistd.h>

/*
pPetri petriCreate(unsigned int nb_pl, unsigned int nb_tr);
void petriAddPlace(pPetri net, unsigned int index, unsigned int initial_marking);
void petriAddTransition(pPetri net, unsigned int index);
void petriAddlink(pPetri net, int input_type, unsigned int input, int output_type, unsigned int output, int weight);
void petriFree(pPetri p);
 */
int main(int argc, char ** argv)
{
	/*pArray2 p = NULL;
	pArray2 data;
	pArray2 l;
	p = add2(p, uInt_t, uIntCreateNode(1));
	l = p;
	p = add2(p, uInt_t, uIntCreateNode(2));
	p = add2(p, uInt_t, uIntCreateNode(3));
	data = p;
	p = add2(p, uInt_t, uIntCreateNode(4));
	

	displaySimpeNodeArray2(p);
	pArray2 tmp = removeElemArray2(&p, data);
	displaySimpeNodeArray2(l);

	freeNodeArray2(tmp);
	freeArray2(p);*/

	/*pPetriNode node = petriNodeCreate();
	pArray2 p1, p2, p, tmp;
	p = NULL;
	p = add2(p, uInt_t, uIntCreateNode(1));
	p1 = p;
	p = add2(p, uInt_t, uIntCreateNode(2));
	p2 = p;

	petriNodeAddInput(node, p1);
	petriNodeAddInput(node, p2);

	void displayNode(pPetriNode node)
	{
		pArray l;
		l = node->input_links;
		while(l)
		{
			tmp = (pArray2)(l->data);
			printf("{%d} - ", ((pInt)tmp->data)->data);
			l = l->next;
		}
		printf("\n");
	}

	displayNode(node);

	petriNodeRemoveInput(node, p1);

	displayNode(node);

	petriNodeFree(node);

	
	freeArray2(p);*/


	

	pPetri net = petriCreate(5, 3);
	petriAddPlace(net, 0, 2);
	petriAddPlace(net, 1, 2);
	petriAddPlace(net, 3, 2);

	petriAddTransition(net, 0);
	petriAddTransition(net, 1);
	petriAddTransition(net, 2);

	petriAddlink(net, PETRI_PLACE_TYPE, 0, PETRI_TRANSITION_TYPE, 0, 5);
	petriAddlink(net, PETRI_PLACE_TYPE, 0, PETRI_TRANSITION_TYPE, 1, 4);
	petriAddlink(net, PETRI_PLACE_TYPE, 1, PETRI_TRANSITION_TYPE, 1, 4);

	petriAddlink(net, PETRI_TRANSITION_TYPE, 1, PETRI_PLACE_TYPE, 3, 6);
	petriAddlink(net, PETRI_TRANSITION_TYPE, 2, PETRI_PLACE_TYPE, 1, 6);
	petriAddlink(net, PETRI_TRANSITION_TYPE, 2, PETRI_PLACE_TYPE, 3, 6);

	displayPetriNet(net);

	petriRemovePlace(net, 1);
	petriRemoveTransition(net, 1);

	/*pArray2 p1, p2;

	p1 = (pArray2)((net->places[0]->output_links)->data);
	p2 = (pArray2)(((net->transitions[1]->input_links)->next)->data);

	printf("p1 = ");
	displayPetriLink(p1->data);
	printf("\n");
	printf("p2 = ");
	displayPetriLink(p2->data);
	printf("\n");

	printf("p1==p2 ? %d\n", (p1==p2));

	petriNodeRemoveOutput(net->places[0], p2);*/
	displayPetriNet(net);

	petriFree(net);


	//test functions
	
	//srand(time(NULL));
	
	/*unsigned int n = 10;
	pArray * tree = buildTree(n, 3);
	stronglyConnectedGraph(tree, n, 1);
	
	pPetri p = petriTransformation(tree, n);
	char *s=NULL;
	displayMatrix(p->trans, p->num_tr, p->num_pl);
	
	petriToString(&s, p);
	
	
	fprintf(stderr, "res=%s\n", s);
	free(s);
	
	freeList(tree, n);
	petriFree(p);*//*
	srand(getpid());
	unsigned int size = 10;
	unsigned int Ki = 4;
	unsigned int Ko = 3;

	pDirectedGraph graph = randomGraph(size, Ki, Ko);
	displayGraph(graph->links_list, size);
	stronglyConnectedGraph(graph, 0);
	printf("\n----------------------------------\n");
	displayGraph(graph->links_list, size);

	directedGraphFree(graph);

	unsigned int * input = weightsComputation(10, 100);
	free(input);
	return 0;
	
	//pArray * graph = randomGraph(size, Ki, Ko);
	//displayGraph(graph, size);
	
	//freeList(graph, size);
	
	pDirectedGraph temp = randomGraph(size, Ki, Ko);//create graph
	stronglyConnectedGraph(temp, 0);
	
	displayGraph(temp->links_list, size);
	printf("\n\n-------------\n\n");
	
	pPetri p = petriTransformation(temp);
	//displayMatrix(p->trans, p->num_tr, p->num_pl);
	
	
	directedGraphFree(temp);
	petriFree(p);
	*/
	return 0;
}


