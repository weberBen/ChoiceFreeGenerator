#ifndef _GRAPH_GENERATOR_H
#define _GRAPH_GENERATOR_H

#include "Array.h"
#include "Rand.h"

pArray * forcedTree(unsigned int n, unsigned int D);
pArray * buildTree(unsigned int n, unsigned int D);
void stronglyConnectedGraph(pArray * tree, unsigned int n);

#endif 
