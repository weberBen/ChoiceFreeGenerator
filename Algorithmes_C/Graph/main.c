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


	

	pPetri net = petriCreate(5, 5);
	petriAddPlace(net, 0, 2);
	petriAddPlace(net, 1, 24);
	petriAddPlace(net, 2, 22);
	petriAddPlace(net, 3, 23);
	petriAddPlace(net, 4, 21);

	petriAddTransition(net, 0);
	petriAddTransition(net, 1);
	petriAddTransition(net, 2);
	petriAddTransition(net, 3);
	petriAddTransition(net, 4);

	petriAddlink(net, PETRI_PT_LINK, 0, 4, 75);
	petriAddlink(net, PETRI_PT_LINK, 1, 4, 52);
	petriAddlink(net, PETRI_PT_LINK, 2, 4, 54);
	petriAddlink(net, PETRI_PT_LINK, 3, 4, 12);

	petriAddlink(net, PETRI_TP_LINK, 0, 0, 62);
	petriAddlink(net, PETRI_TP_LINK, 1, 1, 26);
	petriAddlink(net, PETRI_TP_LINK, 2, 2, 67);
	petriAddlink(net, PETRI_TP_LINK, 3, 3, 36);
	petriAddlink(net, PETRI_TP_LINK, 4, 4, 63);

	displayPetriNet(net);

	int sum=0;
	int weight1, weight2;
	
	weight2 = petriGetWeightLink(net, PETRI_PT_LINK, 0, 4);
	sum+=weight2;

	weight1 = petriGetWeightLink(net, PETRI_TP_LINK, 1, 1);
	weight2 = petriGetWeightLink(net, PETRI_PT_LINK, 1, 4);
	sum+=weight2;
	petriRemovePlace(net, 1);
	petriAddlink(net, PETRI_TP_LINK, 1, 0, weight1);

	weight1 = petriGetWeightLink(net, PETRI_TP_LINK, 2, 2);
	weight2 = petriGetWeightLink(net, PETRI_PT_LINK, 2, 4);
	sum+=weight2;
	petriRemovePlace(net, 2);
	petriAddlink(net, PETRI_TP_LINK, 2, 0, weight1);

	weight1 = petriGetWeightLink(net, PETRI_TP_LINK, 3, 3);
	weight2 = petriGetWeightLink(net, PETRI_PT_LINK, 3, 4);
	sum+=weight2;
	petriRemovePlace(net, 3);
	petriAddlink(net, PETRI_TP_LINK, 3, 0, weight1);

	printf("sum=%d\n", sum);
	//petriRemovePlace()
	displayPetriNet(net);
	
	printf("initial marking =%d\n", petriGetInitialMarking(net, 0));
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


