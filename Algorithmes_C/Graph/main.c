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
	
	unsigned int n = 10;
	pArray * tree = buildTree(n, 3);
	stronglyConnectedGraph(tree, n, 1);
	
	pPetri p = petriTransformation(tree, n);
	char *s=NULL;
	displayMatrix(p->trans, p->num_tr, p->num_pl);
	
	petriToString(&s, p);
	
	
	fprintf(stderr, "res=%s\n", s);
	free(s);
	
	freeList(tree, n);
	petriFree(p);
	
	return 0;
}


