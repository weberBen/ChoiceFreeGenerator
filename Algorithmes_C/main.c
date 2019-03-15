#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "GraphGenerator.h"
#include "Tools.h"
#include "Python.h"

int main()
{
	srand(time(NULL));
	unsigned int n = 50;
	
	pArray * tree = buildTree(n, 5);
	displayList(tree, n);
	
	stronglyConnectedGraph(tree, n);
	
	printf("\n--------------------------\n");
	displayList(tree, n);
	
	
	listToFile("test.txt", tree, n);
	freeList(tree, n);
	
	printf("\n--------------------------\n");
	py_createStronglyConnectedGraph(50, 5, "test.txt");
	
	return 0;
}
