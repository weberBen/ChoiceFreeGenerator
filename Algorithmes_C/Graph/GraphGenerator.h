#ifndef _GRAPH_GENERATOR_H
#define _GRAPH_GENERATOR_H

#include <glpk.h> 

#include "Array.h"
#include "Rand.h"
#include "Tools.h"
#include "Display.h"

#define GLPK_SIZE 1+100000 //large size requires for graph with nodes between 1000 and 10 000

//create simple directed graph
pDirectedGraph randomGraph(unsigned int n, int Ki, int Ko);
pDirectedGraph buildTree(unsigned int n, unsigned int D);

//get strongly connect graph from a directed graph
void stronglyConnectedGraph(pDirectedGraph graph, int isTree);

//convert a strongly connect graph into an SDF
pPetri petriTransformation(pDirectedGraph graph);//no normalization (all the weight set to 0)
pPetri petriNormalizedTransformation(unsigned int * real_vect_norm,
									 pDirectedGraph graph, unsigned int repetition_vect_norm);//from the norm of the repetition vector, generate a random vector and used it to normalize the SDF

//Initialization of SDF
unsigned int * weightsComputation(unsigned int * real_vect_norm,
                                  unsigned int nb_transition, unsigned int repetition_vect_norm);//create random repetition vector (where the gcd of the array is 1)
void setInitialMarking(pPetri net);//use glpk to create a liviness initial marking

//SDF to Free-choice transformation
void sdfToFreeChoice(pPetri net, int resizeNetAfter);

//generate random Free-choice
/* The transformation from SDF to Free-choice (used to generate a random Free-choice) leave empty memory space inside the petri structure
   If "cleanExtraMemSpace" is set to 0, then these empty space will remain in the petri net after the transformation (somme value of
   the fixed size arrays inside the petri structure will be set to NULL)
   Else the petri net will be resized, which can take extra time
*/
pPetri generateRandomFreeChoice(unsigned int * real_vect_norm, 
								unsigned int nb_transition, unsigned int nb_input_node, unsigned int nb_output_node, unsigned int repetition_vect_norm, int cleanExtraMemSpace);//Free-choice from its repetition vector norm
pPetri generateFreeChoiceWithVector(unsigned int nb_transition, unsigned int nb_input_node, unsigned int nb_output_node, unsigned int * repetition_vect, int cleanExtraMemSpace);//Free-choice from its repetition vector

#endif 
