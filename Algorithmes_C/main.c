#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "GraphGenerator.h"
#include "Tools.h"


int main()
{
	//srand(time(NULL));
	unsigned int n = 20;
	
	pArray * tree = buildTree(n, 5);
	displayList(tree, n);
	
	char * txt;
	unsigned int m = listToString(&txt, tree, n);
	printf("res=%s\nsize=%u\n", txt, m);
	free(txt);
	
	listToFile("test.txt", tree, n);
	
	freeList(tree, n);
	
	return 0;
}
