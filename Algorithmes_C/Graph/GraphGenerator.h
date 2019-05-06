#ifndef _GRAPH_GENERATOR_H
#define _GRAPH_GENERATOR_H

#include "Array.h"
#include "Rand.h"

pArray * randomGraph(unsigned int n, int Ki, int Ko);

pArray * buildTree(unsigned int n, unsigned int D);

void stronglyConnectedGraph(pArray * tree, unsigned int n, int isTree);

pPetri petriTransformation(pArray *graph, unsigned int size);

#endif 
