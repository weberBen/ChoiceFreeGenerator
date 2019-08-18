#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/resource.h>
#include <string.h>

#include "GraphGenerator.h"
#include "Display.h"

#define NAME "freeChoiceGenerator"
#define NUMBER_ARGS 3


/*
	//-----------------------------------------------------------------------------------------------------------------
	//										
	//											SIMPLE TUTORIAL
	//
	//-----------------------------------------------------------------------------------------------------------------


	srand(time(NULL));

	//-----------------------------------------------------------------------------------------------------------------
	//										GENERATE RANDOM FREE CHOICE 
	//-----------------------------------------------------------------------------------------------------------------

	unsigned int nb_transition = 10;//number of desired transition in the result Free-choice
	unsigned int nb_input_node = 3;//average (and maximum) number of inputs for each transition
	unsigned int nb_output_node = 3;//average (and maximum) number of outputs for each transition
	unsigned int vect_norm = 10;//norm of the repetition vector to generate
	int cleanExtraMem = 0;//the transformation from SDF to Free-choice leaves extra empty memory space, the cleaning process is optional because it's time consuming

	//create random Free-choice from its repetition vector norm
	pPetri net1 = generateRandomFreeChoice(nb_transition, nb_input_node, nb_output_node, vect_norm, cleanExtraMem);
	//write Free-choice named "net1" to file "net1.pnml"
	petriToPnmlFile(net1, "net1", "net1.pnml");
	petriFree(net1);

	//create random Free-choice from a given repetition vector
	unsigned int * vect = weightsComputation(nb_transition, vect_norm);
	pPetri net2 = generateFreeChoiceWithVector(nb_transition, nb_input_node, nb_output_node, vect, cleanExtraMem);
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
*/


void printHelp()
{
	printf("NAME\n" \
			NAME " - a random generator of living Free-choice\n" \
			"\n" \
			"SYNOPSIS\n" \
			"\t" NAME " [nb_transition] [nb_input_node] [vect_norm] -OPTIONS\n" \
			"\n" \
			"DESCRIPTION\n" \
			"\t" NAME " allows user to generate random living Free-choice network given the desired number of transitions [nb_transition] " \
			"\t" "in the final petri network. The generation of the final network is based uppon the one of a strongly connected " \
			"\t" "graph where the number of input [nb_input_node] and output [nb_output_node] per node is set by " \
			"\t" "the user (with the following constraint [nb_input_node]>=[nb_output_node]). By default [nb_input_node]=[nb_output_node].\n" \
			"\t" "The [vect_norm] is the desired norm of the repetition vector to get during the generation of a random one for the SDF\n" \
			"\n" \
			"OPTIONS\n" \
			"\t" "-o [nb_output_node]   " "set [nb_output_node]" "\n" \
			"\t" "-c   " "resize the petri net after the transformation from SDF to Free-choice (can be time consuming)" "\n" \
			"\t" "-f [filename]   " "save the Free-choice to a file to [filename] as PNML format" "\n" \
			"\t" "-s [stack_size]   " "set the new stack size to use" "\n" \
			"\t" "-h   " "get help" "\n");
}

int main(int argc, char ** argv)
{
	unsigned int nb_transition = 0;//number of desired transition in the result Free-choice
	unsigned int nb_input_node = 0;//average (and maximum) number of inputs for each transition
	unsigned int nb_output_node = 0;//average (and maximum) number of outputs for each transition
	unsigned int vect_norm = 0;//norm of the repetition vector to generate
	unsigned int real_vect_norm;
	unsigned int stack_size = 0;
	int cleanExtraMem = 0;//the transformation from SDF to Free-choice leaves extra empty memory space, the cleaning process is optional because it's time consuming
	char * filename = NULL;
	unsigned int nb_regular_arg = 0;
	int c;

    while((c = getopt(argc, argv, "o:cf:s:h")) != -1) 
	{
		// Option argument
		switch (c) 
		{
			case 'o': 
				nb_output_node = (int)atol(optarg);
				break;
			case 'c': 
				cleanExtraMem = 1;
				break;
			case 'f':
				filename = optarg;
				break;
			case 'h':
			{	
				printHelp();
				return 0;
			}
				break;
			case 's':
				stack_size = (int)atol(optarg);
				break;
			default:
				break;
		}
	}

	int index;
 	for (index = optind; index < argc; index++)
	{
		if(nb_regular_arg==0)
			nb_transition = (int)atol(argv[index]);
		else if (nb_regular_arg==1)
			nb_input_node = (int)atol(argv[index]);
		else if (nb_regular_arg==2)
			vect_norm = (int)atol(argv[index]);
		
		nb_regular_arg++;
	}
	

	if(nb_regular_arg != NUMBER_ARGS)
	{
		printf("Wrong arguments !\n-h to get help\n");
		return 1;
	}


	/* The structure used to store petri net is heavy in memory (to make the transformation easier)
	   For a lot of elements inside the petri net the default allocate stack memory must be increased
	*/
	if(stack_size!=0)
	{
		struct rlimit lim = {stack_size, stack_size};

		if (setrlimit(RLIMIT_STACK, &lim) == -1)
			return 1;

		printf("Increase stack size to %u\n", stack_size);
	}
	

	if(nb_output_node==0)
		nb_output_node = nb_input_node;
	

	//create random Free-choice from its repetition vector norm
	printf("Generation of a random Free-choice with : \n"\
		   "\t" "[nb_transition] = %u \n" \
		   "\t" "[nb_input_node] = %u \n" \
		   "\t" "[nb_output_node] = %u \n" \
		   "\t" "[vect_norm] = %u \n" \
		   "\t" "[cleanExtraMem] = %s \n" \
		   , nb_transition, nb_input_node, nb_output_node, vect_norm, (cleanExtraMem)?"yes":"no");
	
	//create Free-choice
	srand(time(NULL));
	
	pPetri net = generateRandomFreeChoice(&real_vect_norm, nb_transition, nb_input_node, nb_output_node, vect_norm, cleanExtraMem);
	//write Free-choice named "net1" to file "net1.pnml"
	if(filename!=NULL)
	{	printf("Save generated Free-choice to %s\n", filename);
		petriToPnmlFile(net, "Free-choice", filename);
	}else
	{
		petriToPnmlDisplay(net, "Free-choice");
	}
	//free memory
	petriFree(net);

	return 0;
}

