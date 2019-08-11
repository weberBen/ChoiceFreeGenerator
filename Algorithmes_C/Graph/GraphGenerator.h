#ifndef _GRAPH_GENERATOR_H
#define _GRAPH_GENERATOR_H

#include "Array.h"
#include "Rand.h"
#include "Tools.h"
#include "Display.h"

pDirectedGraph randomGraph(unsigned int n, int Ki, int Ko);

pDirectedGraph buildTree(unsigned int n, unsigned int D);

void stronglyConnectedGraph(pDirectedGraph graph, int isTree);

pPetri petriTransformation(pDirectedGraph graph);

pFixedSizeList weightsComputation(unsigned int nb_transition, unsigned int repetition_vect_norm);
void normalizationPetriNetwork(pPetri net, pFixedSizeList repetition_vect);

#endif 
