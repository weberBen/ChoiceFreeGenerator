#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "GraphGenerator.h"
#include "Tools.h"
#include "Python.h"


int main(int argc, char ** argv)
{
    if(startPyFunctions(argc, argv)==0)
    {
		return 0;
	}
	
	//test functions
	srand(time(NULL));
	/*unsigned int n = 10;
	
	pArray * tree = buildTree(n, 5);
	displayList(tree, n);
	
	stronglyConnectedGraph(tree, n);
	
	printf("\n--------------------------\n");
	displayList(tree, n);
	
	
	listToFile("test.txt", tree, n);
	freeList(tree, n);*/
	
	printf("\n--------------------------\n");
	py_createStronglyConnectedGraph(10, 5, "testo.txt");
	
	return 0;
}
