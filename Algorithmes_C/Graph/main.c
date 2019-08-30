#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <sys/resource.h>
#include <string.h>

#include "GraphGenerator.h"
#include "Display.h"
#include "Tools.h"

#define NAME "choiceFreeGenerator"
#define NUMBER_ARGS 3


void printHelp()
{
	printf("NAME\n" \
			"\t" NAME " - a random generator of living Choice-Free\n" \
			"\n" \
			"SYNOPSIS\n" \
			"\t" NAME " [nb_transition] [avg_input_node] [vect_norm] -OPTIONS\n" \
			"\n" \
			"DESCRIPTION\n" \
			"\t" NAME " allows user to generate random living Choice-Free network given the desired number of transitions [nb_transition] " "\n" \
			"\t" "in the final petri network." "\n" \
			"\t" "The generation of the final network is based uppon the one of a strongly connected " \
				 "graph where the average number of input [avg_input_node] and output [avg_output_node] is set by " \
				 "the user (with the following constraint [avg_input_node]>=[avg_output_node]). By default [avg_input_node]=[avg_output_node]." "\n" \
			"\t" "The [vect_norm] is the desired norm of the repetition vector to get during the generation of a random one for the SDF\n" "\n" \
			"\n" \
			"OPTIONS\n" \
			"\t" "-o [avg_output_node]   " "set [avg_output_node]" "\n" \
			"\t" "-c   " "resize the petri net after the transformation from SDF to Choice-Free (can be time consuming)" "\n" \
			"\t" "-f [filename]   " "save the Choice-Free to a file to [filename] as PNML format" "\n" \
			"\t" "-h   " "get help" "\n");
}

int main(int argc, char ** argv)
{


	//initialize structure
	pPetri net3 =  petriCreate(3, 4);
	
	//add places
	petriAddPlace(net3, 0, 5);
	petriAddPlace(net3, 1, 4);
	petriAddPlace(net3, 2, 8);
	
	petriAddTransition(net3, 0);
	petriAddTransition(net3, 1);
	petriAddTransition(net3, 2);
	petriAddTransition(net3, 3);

	
	petriAddlink(net3, PETRI_PT_LINK, 0, 3, 7);
	petriAddlink(net3, PETRI_PT_LINK, 1, 3, 7);
	petriAddlink(net3, PETRI_PT_LINK, 2, 3, 7);

	petriAddlink(net3, PETRI_TP_LINK, 0, 0, 4);
	petriAddlink(net3, PETRI_TP_LINK, 1, 1, 1);
	petriAddlink(net3, PETRI_TP_LINK, 2, 2, 2);


	displayPetriNet(net3);
	printf("\n---------------------------------------------------\n");
	sdfToChoiceFree(net3, 0);
	displayPetriNet(net3);

	petriFree(net3);

	return 0;
	unsigned int nb_transition = 0;//number of desired transition in the result Choice-Free
	unsigned int avg_input_node = 0;//average (and maximum) number of inputs for each transition
	unsigned int avg_output_node = 0;//average (and maximum) number of outputs for each transition
	unsigned int vect_norm = 0;//norm of the repetition vector to generate
	unsigned int real_vect_norm;
	int cleanExtraMem = 0;//the transformation from SDF to Choice-Free leaves extra empty memory space, the cleaning process is optional because it's time consuming
	char * filename = NULL;
	unsigned int nb_regular_arg = 0;
	int c;

    while((c = getopt(argc, argv, "o:cf:s:h")) != -1) 
	{
		// Option argument
		switch (c) 
		{
			case 'o': 
				avg_output_node = (int)atol(optarg);
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
			avg_input_node = (int)atol(argv[index]);
		else if (nb_regular_arg==2)
			vect_norm = (int)atol(argv[index]);
		
		nb_regular_arg++;
	}
	

	if(nb_regular_arg != NUMBER_ARGS)
	{
		printf("Wrong arguments !\n-h to get help\n");
		return 1;
	}
	
	if(avg_output_node==0)
		avg_output_node = avg_input_node;
	

	//create random Choice-Free from its repetition vector norm
	printf("Generation of a random Choice-Free with : \n"\
		   "\t" "[nb_transition] = %u \n" \
		   "\t" "[nb_input_node] = %u \n" \
		   "\t" "[nb_output_node] = %u \n" \
		   "\t" "[vect_norm] = %u \n" \
		   "\t" "[cleanExtraMem] = %s \n" \
		   , nb_transition, avg_input_node, avg_output_node, vect_norm, (cleanExtraMem)?"yes":"no");
	
	//create Choice-Free
	srand(time(NULL));
	
	pPetri net = generateRandomChoiceFree(&real_vect_norm, nb_transition, avg_input_node, avg_output_node, vect_norm, cleanExtraMem);
	if(net==NULL)
	{
		printf("Cannot generate a random Choice-Free\n");
		return 1;
	}
	//write Choice-Free named "net1" to file "net1.pnml"
	if(filename!=NULL)
	{	printf("Save generated Choice-Free to %s\n", filename);
		petriToPnmlFile(net, "Choice-Free", filename);
	}else
	{
		petriToPnmlDisplay(net, "Choice-Free");
	}
	//free memory
	petriFree(net);

	return 0;
}

