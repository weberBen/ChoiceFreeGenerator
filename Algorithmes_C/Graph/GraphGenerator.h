#ifndef _GRAPH_GENERATOR_H
#define _GRAPH_GENERATOR_H

#include <glpk.h> 

#include "Array.h"
#include "Rand.h"
#include "Tools.h"
#include "Display.h"

 pDirectedGraph matrixIntoList(int network[], unsigned int n);
 void connectedGraph(int network[], unsigned int size);
//create simple directed graph
pDirectedGraph randomConnectedGraph(unsigned int n, double density);
pDirectedGraph buildTree(unsigned int n, unsigned int D);

//get strongly connect graph from a directed graph
void stronglyConnectedGraph(pDirectedGraph graph, int isTree);

//convert a strongly connect graph into an SDF
void normalizationPetriNetwork(pPetri net, unsigned int * repetition_vect);
pPetri petriTransformation(pDirectedGraph graph);//no normalization (all the weight set to 0)
pPetri petriNormalizedTransformation(unsigned int * real_vect_norm,
									 pDirectedGraph graph, unsigned int repetition_vect_norm);//from the norm of the repetition vector, generate a random vector and used it to normalize the SDF

//Initialization of SDF
unsigned int * weightsComputation(unsigned int * real_vect_norm,
                                  unsigned int nb_transition, unsigned int repetition_vect_norm);//create random repetition vector (where the gcd of the array is 1)
void setInitialMarking(pPetri net);//use glpk to create a liviness initial marking

//SDF to Free-choice transformation
void sdfToChoiceFree(pPetri net, int resizeNetAfter);

//generate random Free-choice
/* The transformation from SDF to Free-choice (used to generate a random Free-choice) leave empty memory space inside the petri structure
   If "cleanExtraMemSpace" is set to 0, then these empty space will remain in the petri net after the transformation (somme value of
   the fixed size arrays inside the petri structure will be set to NULL)
   Else the petri net will be resized, which can take extra time
*/
pPetri generateRandomChoiceFree(unsigned int * real_vect_norm, 
								unsigned int nb_transition, double density, unsigned int repetition_vect_norm, int SDF, int cleanExtraMemSpace);//Free-choice from its repetition vector norm
pPetri generateChoiceFreeWithVector(unsigned int nb_transition, double density, unsigned int * repetition_vect, int SDF, int cleanExtraMemSpace);//Free-choice from its repetition vector

#endif 
