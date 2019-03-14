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
