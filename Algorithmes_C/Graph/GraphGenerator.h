#ifndef _GRAPH_GENERATOR_H
#define _GRAPH_GENERATOR_H

#include <glpk.h> 

#include "Array.h"
#include "Rand.h"
#include "Tools.h"
#include "Display.h"

#define GLPK_SIZE 1+1000

pDirectedGraph randomGraph(unsigned int n, int Ki, int Ko);

pDirectedGraph buildTree(unsigned int n, unsigned int D);

void stronglyConnectedGraph(pDirectedGraph graph, int isTree);

pPetri petriTransformation(pDirectedGraph graph);
pPetri petriNormalizedTransformation(pDirectedGraph graph, unsigned int repetition_vect_norm);

unsigned int * weightsComputation(unsigned int nb_transition, unsigned int repetition_vect_norm);
void normalizationPetriNetwork(pPetri net, unsigned int * repetition_vect);

void setInitialMarking(pPetri net);

void sdfToFreeChoice(pPetri net, int resizeNetAfter);

pPetri generateRandomFreeChoice(unsigned int nb_transition, unsigned int nb_input_tr, unsigned int nb_output_tr, unsigned int repetition_vect_norm, int cleanExtraMemSpace);
pPetri generateFreeChoiceWithVector(unsigned int nb_transition, unsigned int nb_input_tr, unsigned int nb_output_tr, unsigned int * repetition_vect, int cleanExtraMemSpace);

#endif 
