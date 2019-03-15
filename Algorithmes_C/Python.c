#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Python.h"

void py_createTree(unsigned int n, unsigned int D, char * filename)
{
	pArray * tree = buildTree(n, D);
	listToFile(filename, tree, n);
	freeList(tree, n);
}

void py_createStronglyConnectedGraph(unsigned int n, unsigned int D, char * filename)
{
	pArray * tree = buildTree(n, D);
	stronglyConnectedGraph(tree, n);
	listToFile(filename, tree, n);
	freeList(tree, n);
}
