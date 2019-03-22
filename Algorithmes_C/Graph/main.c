#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "GraphGenerator.h"
#include "Tools.h"
#include "Rand.h"

int main(int argc, char ** argv)
{
	//test functions
	
	srand(time(NULL));

	
	unsigned int n = 7;
	
	pArray * tree = forcedTree(n, 3);
	displayGraph(tree, n);
	
	stronglyConnectedGraph(tree, n);
	
	//printf("\n--------------------------\n");
	//displayList(tree, n);
	char * s;
	listToString(&s,tree, n);//convert the tree into a string
	fprintf(stderr, "abre=%s\n", s);
	
	//listToFile("test.txt", tree, n);
	free(s);
	
	freeList(tree, n);
	
	return 0;
}


