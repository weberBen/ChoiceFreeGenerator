#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "choiceFreeGeneratorDev.h"

int main()
{
	srand(time(NULL));

	//-----------------------------------------------------------------------------------------------------------------
	//										GENERATE RANDOM CHOICE-FREE
	//-----------------------------------------------------------------------------------------------------------------

	unsigned int nb_transition = 10;//number of desired transition in the result Choice-Free
	double density = 0.5;//density of the graph
	unsigned int vect_norm = 10;//norm of the repetition vector to generate
	int cleanExtraMem = 0;//the transformation from SDF to Choice-Free leaves extra empty memory space, the cleaning process is optional because it's time consuming
	unsigned int real_vect_norm = 0;
	int SDF = 0;

	//create random Choice-Free from its repetition vector norm
	pPetri net1 = generateRandomChoiceFree(&real_vect_norm, nb_transition, density, vect_norm, SDF, cleanExtraMem);
	if(net1==NULL)
	{
		printf("Error during the generation of a random Choice-Free\n");
		return 1;
	}
	printf("Real repetition vector norm : %u\n", real_vect_norm);
	//write Choice-Free named "net1" to file "net1.pnml"
	petriToPnmlFile(net1, "net1", "net1.pnml");
	petriFree(net1);

	//create random Choice-Free from a given repetition vector
	unsigned int * vect = weightsComputation(&real_vect_norm, nb_transition, vect_norm);
	if(vect==NULL)
	{
		printf("Error during the generation of a random repetition vector\n");
		return 1;
	}
	printf("Real repetition vector norm : %u\n", real_vect_norm);
	pPetri net2 = generateChoiceFreeWithVector(nb_transition, density, vect, SDF, cleanExtraMem);
	if(net2==NULL)
	{
		printf("Error during the generation of a random Choice-Free\n");
		free(vect);
		return 1;
	}
	//write Choice-Free named "net2" to file "net2.pnml"
	petriToPnmlFile(net2, "net2", "net2.pnml");
	free(vect);
	petriFree(net2);


	//-----------------------------------------------------------------------------------------------------------------
	//										TRANSFORMATION EXAMPLE
	//-----------------------------------------------------------------------------------------------------------------

	//generate more places and transitions that needed to have "random" index for the places and transitions of the transformation
	unsigned int nb_pl_transfo = 5;
	unsigned int weight = 7;
	unsigned int end_tr = nb_pl_transfo;
	unsigned int id_pl, id_tr;
	pPetri net3 =  petriCreate(nb_pl_transfo, nb_pl_transfo+1);

	unsigned int k;

	petriAddTransition(net3, end_tr);
	for(k=0; k<nb_pl_transfo; k++)
	{
		id_pl = k;
		petriAddPlace(net3, id_pl, rand()%10);
		id_tr = k;
		petriAddTransition(net3, id_tr);

		petriAddlink(net3, PETRI_TP_LINK, id_tr, id_pl, rand()%10);
		petriAddlink(net3, PETRI_PT_LINK, id_pl, end_tr, weight);
	}

	displayPetriNet(net3);

	sdfToChoiceFree(net3, 0);

	displayPetriNet(net3);

	petriFree(net3);

	//-----------------------------------------------------------------------------------------------------------------
	//										CREATE A PETRI NET
	//-----------------------------------------------------------------------------------------------------------------

	unsigned int nb_place = 5;
	unsigned int nb_transitions = 5;
	int i;

	//initialize structure
	pPetri net4 =  petriCreate(nb_place, nb_transitions);
	
	//add places
	petriAddPlace(net4, 0, 1);//add place 0 with an initial marking of 1
	petriAddPlace(net4, 3, 2);//add place 3 with an initial marking of 2
	petriAddPlace(net4, 4, 3);//add place 4 with an initial marking of 3
	petriAddPlace(net4, 1, 4);//add place 1 with an initial marking of 4
	//the place 2 will not be set inside the petri structure (it value inside the fixed size array will be set to NULL)

	//add transitions
	for(i=0; i<nb_transitions; i++)
	{
		petriAddTransition(net4, i);//add transition i
	}

	//add links
	petriAddlink(net4, PETRI_PT_LINK, 0, 1, 12);//add link between the place 0 and the transition 1 with a weight of 12
	petriAddlink(net4, PETRI_PT_LINK, 1, 3, 0);//add link between the place 1 and the transition 3 with a weight of 0

	petriAddlink(net4, PETRI_TP_LINK, 4, 3, 4);//add link between the transition 4 and the place 3 with a weight of 4
	petriAddlink(net4, PETRI_TP_LINK, 3, 2, 1);//add link between the transition 3 and the place 2 with a weight of 1
	petriAddlink(net4, PETRI_TP_LINK, 3, 0, 5);//add link between the transition 3 and the place 0 with a weight of 5


	petriAddlink(net4, PETRI_PT_LINK, 2, 1, 12);//not possible because place 2 does not exists
	petriAddlink(net4, PETRI_TP_LINK, 3, 2, 12);//not possible because place 2 does not exists

	pPetriLink link = petriGetLink(net4, PETRI_PT_LINK, 0, 1);//get link between the place 0 and the transition 1
	printf("Weight of link between place 0 and transition 1 : %d\n", link->weight);
	printf("Weight of link between transition 4 and place 3 : %d\n", petriGetWeightLink(net4, PETRI_TP_LINK, 4, 3));


	pPetriElem elem = petriGetPlace(net4, 0);//get place 0
	printf("Initial marking of place %u : %d\n", elem->label, elem->val);
	printf("Initial marking of place 4 : %d\n", petriGetInitialMarking(net4, 4));

	printf("Places : { ");
	for(i=0; i<net4->nb_pl; i++)
	{
		displayPetriElem(net4->pl_elems[i]);
		printf(" ");
	}
	printf("}\n");

	//get all the link for a node
	pPetriNode node = net4->places[0];
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
	petriRemovePlace(net4, 3);
	printf("Places : { ");
	for(i=0; i<net4->nb_pl; i++)
	{
		displayPetriElem(net4->pl_elems[i]);
		printf(" ");
	}
	printf("}\n");

	//clear extra memory space
	petriClearPlaces(net4);

	printf("Places : { ");
	for(i=0; i<net4->nb_pl; i++)
	{
		displayPetriElem(net4->pl_elems[i]);
		printf(" ");
	}
	printf("}\n");

	//free memory
	petriFree(net4);
	
	return 0;
}
