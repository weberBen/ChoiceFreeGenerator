#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "GraphGenerator.h"
#include "Tools.h"
#include "Python.h"

#include "Server.h"

/*int main(int argc, char ** argv)
{
    if(startPyFunctions(argc, argv)==0)
    {
		return 0;
	}
	
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
}*/

int main()
{
	int PORT = 2400;
    int BUFFSIZE = 12;
    
	py_establishCommunication(PORT, BUFFSIZE);
	
	return 0;
}


