#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "GraphGenerator.h"
#include "Tools.h"


int main(int argc, char ** argv)
{
	//test functions
	
	
	srand(time(NULL));
	unsigned int n = 100;
	
	pArray * tree = buildTree(n, 8);
	//displayList(tree, n);
	
	stronglyConnectedGraph(tree, n);
	
	//printf("\n--------------------------\n");
	//displayList(tree, n);
	char * s;
	listToString(&s,tree, n);//convert the tree into a string
	fprintf(stderr, "abre=%s\n", s);
	
	//listToFile("test.txt", tree, n);
	freeList(tree, n);
	free(s);
	
	return 0;
}


