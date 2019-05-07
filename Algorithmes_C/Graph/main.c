#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "GraphGenerator.h"
#include "Tools.h"
#include "Rand.h"

int main(int argc, char ** argv)
{
	
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
	petriFree(p);*/
	
	unsigned int size = 10;
	unsigned int Ki = 4;
	unsigned int Ko = 3;
	
	//pArray * graph = randomGraph(size, Ke, Ko);
	//displayGraph(graph, size);
	
	//freeList(graph, size);
	
	pArray * temp = randomGraph(size, Ki, Ko);//create graph
	
	displayGraph(temp, size);
	
	freeList(temp, size);

	return 0;
}


