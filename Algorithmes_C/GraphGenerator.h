#ifndef _GRAPH_GENERATOR_H
#define _GRAPH_GENERATOR_H

#include "Array.h"

typedef struct Graph
{
	pArray * data;
	unsigned int size;
} graph;

void freeGraph(graph * g);


pArray * buildTree(unsigned int n, unsigned int D);
void stronglyConnectedGraph(pArray * tree, unsigned int n);

#endif 
