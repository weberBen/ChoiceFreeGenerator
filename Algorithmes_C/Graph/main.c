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

#define NAME "freeChoiceGenerator"
#define NUMBER_ARGS 3


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
			"\t" "-h   " "get help" "\n");
}

int main(int argc, char ** argv)
{
	
	unsigned int nb_transition = 0;//number of desired transition in the result Free-choice
	unsigned int nb_input_node = 0;//average (and maximum) number of inputs for each transition
	unsigned int nb_output_node = 0;//average (and maximum) number of outputs for each transition
	unsigned int vect_norm = 0;//norm of the repetition vector to generate
	unsigned int real_vect_norm;
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
	if(net==NULL)
	{
		printf("Cannot generate a random Free-choice\n");
		return 1;
	}
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

