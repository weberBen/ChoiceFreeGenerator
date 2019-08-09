#ifndef _GRAPH_GENERATOR_H
#define _GRAPH_GENERATOR_H

#include "Array.h"
#include "Rand.h"
#include "Tools.h"
#include "Display.h"

pDirectedGraph randomGraph(unsigned int n, int Ki, int Ko);

pDirectedGraph buildTree(unsigned int n, unsigned int D);

void stronglyConnectedGraph(pDirectedGraph graph, int isTree);

unsigned int * weightsComputation(unsigned int nb_transition, unsigned int repetition_vect_norm);

pPetri petriTransformation(pDirectedGraph graph);

#endif 
