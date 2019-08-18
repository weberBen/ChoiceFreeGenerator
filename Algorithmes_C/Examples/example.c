#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "freeChoiceGenerator.h"

int main()
{
	srand(time(NULL));

	//-----------------------------------------------------------------------------------------------------------------
	//										GENERATE RANDOM FREE CHOICE 
	//-----------------------------------------------------------------------------------------------------------------

	unsigned int nb_transition = 10;//number of desired transition in the result Free-choice
	unsigned int nb_input_node = 3;//average (and maximum) number of inputs for each transition
	unsigned int nb_output_node = 3;//average (and maximum) number of outputs for each transition
	unsigned int vect_norm = 10;//norm of the repetition vector to generate
	int cleanExtraMem = 0;//the transformation from SDF to Free-choice leaves extra empty memory space, the cleaning process is optional because it's time consuming
	unsigned int real_vect_norm = 0;

	//create random Free-choice from its repetition vector norm
	pPetri net1 = generateRandomFreeChoice(&real_vect_norm, nb_transition, nb_input_node, nb_output_node, vect_norm, cleanExtraMem);
	if(net1==NULL)
	{
		printf("Error during the generation of a random Free-choice\n");
		return 1;
	}
	printf("Real repetition vector norm : %u\n", real_vect_norm);
	//write Free-choice named "net1" to file "net1.pnml"
	petriToPnmlFile(net1, "net1", "net1.pnml");
	petriFree(net1);

	//create random Free-choice from a given repetition vector
	unsigned int * vect = weightsComputation(&real_vect_norm, nb_transition, vect_norm);
	if(vect==NULL)
	{
		printf("Error during the generation of a random repetition vector\n");
		return 1;
	}
	printf("Real repetition vector norm : %u\n", real_vect_norm);
	pPetri net2 = generateFreeChoiceWithVector(nb_transition, nb_input_node, nb_output_node, vect, cleanExtraMem);
	if(net1==NULL)
	{
		printf("Error during the generation of a random Free-choice\n");
		free(vect);
		return 1;
	}
	//write Free-choice named "net2" to file "net2.pnml"
	petriToPnmlFile(net2, "net2", "net2.pnml");
	free(vect);
	petriFree(net2);


	//-----------------------------------------------------------------------------------------------------------------
	//										CREATE A PETRI NET
	//-----------------------------------------------------------------------------------------------------------------

	unsigned int nb_place = 5;
	unsigned int nb_transitions = 5;
	int i;

	//initialize structure
	pPetri net3 =  petriCreate(nb_place, nb_transitions);
	
	//add places
	petriAddPlace(net3, 0, 1);//add place 0 with an initial marking of 1
	petriAddPlace(net3, 3, 2);//add place 3 with an initial marking of 2
	petriAddPlace(net3, 4, 3);//add place 4 with an initial marking of 3
	petriAddPlace(net3, 1, 4);//add place 1 with an initial marking of 4
	//the place 2 will not be set inside the petri structure (it value inside the fixed size array will be set to NULL)

	//add transitions
	for(i=0; i<nb_transitions; i++)
	{
		petriAddTransition(net3, i);//add transition i
	}

	//add links
	petriAddlink(net3, PETRI_PT_LINK, 0, 1, 12);//add link between the place 0 and the transition 1 with a weight of 12
	petriAddlink(net3, PETRI_PT_LINK, 1, 3, 0);//add link between the place 1 and the transition 3 with a weight of 0

	petriAddlink(net3, PETRI_TP_LINK, 4, 3, 4);//add link between the transition 4 and the place 3 with a weight of 4
	petriAddlink(net3, PETRI_TP_LINK, 3, 2, 1);//add link between the transition 3 and the place 2 with a weight of 1
	petriAddlink(net3, PETRI_TP_LINK, 3, 0, 5);//add link between the transition 3 and the place 0 with a weight of 5


	petriAddlink(net3, PETRI_PT_LINK, 2, 1, 12);//not possible because place 2 does not exists
	petriAddlink(net3, PETRI_TP_LINK, 3, 2, 12);//not possible because place 2 does not exists

	pPetriLink link = petriGetLink(net3, PETRI_PT_LINK, 0, 1);//get link between the place 0 and the transition 1
	printf("Weight of link between place 0 and transition 1 : %d\n", link->weight);
	printf("Weight of link between transition 4 and place 3 : %d\n", petriGetWeightLink(net3, PETRI_TP_LINK, 4, 3));


	pPetriElem elem = petriGetPlace(net3, 0);//get place 0
	printf("Initial marking of place %u : %d\n", elem->label, elem->val);
	printf("Initial marking of place 4 : %d\n", petriGetInitialMarking(net3, 4));

	printf("Places : { ");
	for(i=0; i<net3->nb_pl; i++)
	{
		displayPetriElem(net3->pl_elems[i]);
		printf(" ");
	}
	printf("}\n");

	//get all the link for a node
	pPetriNode node = net3->places[0];
	pArray p = node->input_links;//simple linked list
	printf("Inputs of node 0 : \n");
	while(p)
	{
		printf("\t");
		link = petriNodeGetLinkFromArrayNode(p);
		elem = link->input;
		displayPetriElem(elem);
		printf("\n");

		p = p->next;
	}
	printf("Outputs of node 0 : \n");
	p = node->output_links;
	while(p)
	{
		printf("\t");
		link = petriNodeGetLinkFromArrayNode(p);
		elem = link->output;
		displayPetriElem(elem);
		printf("\n");

		p = p->next;
	}

	//remove place 3
	petriRemovePlace(net3, 3);
	printf("Places : { ");
	for(i=0; i<net3->nb_pl; i++)
	{
		displayPetriElem(net3->pl_elems[i]);
		printf(" ");
	}
	printf("}\n");

	//clear extra memory space
	petriClearPlaces(net3);

	printf("Places : { ");
	for(i=0; i<net3->nb_pl; i++)
	{
		displayPetriElem(net3->pl_elems[i]);
		printf(" ");
	}
	printf("}\n");

	//free memory
	petriFree(net3);
	
	return 0;
}
