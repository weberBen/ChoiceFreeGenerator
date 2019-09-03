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
			"\t" NAME " [nb_transition] [density] [vect_norm] -OPTIONS\n" \
			"\n" \
			"DESCRIPTION\n" \
			"\t" NAME " allows user to generate random living Choice-Free network given the desired number of transitions [nb_transition] " "\n" \
			"\t" "in the final petri network." "\n" \
			"\t" "The generation of the final network is based uppon the one of a strongly connected " \
				 "graph whith a density of [density] in ]0, 1]." "\n" \
			"\t" "The [vect_norm] is the desired norm of the repetition vector to get during the generation of a random one for the SDF\n" "\n" \
			"\n" \
			"OPTIONS\n" \
			"\t" "-c   " "resize the petri net after the transformation from SDF to Choice-Free (can be time consuming)" "\n" \
			"\t" "-f [filename]   " "save the Choice-Free to a file to [filename] as PNML format" "\n" \
			"\t" "--sdf  " "produce a SDF as output" "\n" \
			"\t" "--help  " "get help" "\n");
}

static struct option long_options[] =
{
    {"help", no_argument, NULL, 'h'},
    {"sdf", no_argument, NULL, 's'},
    {NULL, 0, NULL, 0}
};


int main(int argc, char ** argv)
{

	unsigned int nb_transition = 0;//number of desired transition in the result Choice-Free
	double density = 0;//average (and maximum) number of inputs for each transition
	unsigned int vect_norm = 0;//norm of the repetition vector to generate
	unsigned int real_vect_norm;
	int cleanExtraMem = 0;//the transformation from SDF to Choice-Free leaves extra empty memory space, the cleaning process is optional because it's time consuming
	int SDF = 0;
	char * filename = NULL;
	unsigned int nb_regular_arg = 0;
	int c;

    while ((c = getopt_long(argc, argv, "cf:sh", long_options, NULL)) != -1)
	{
		// Option argument
		switch (c) 
		{
			case 'c': 
				cleanExtraMem = 1;
				break;
			case 's':
				SDF = 1;
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
			nb_transition = atoi(argv[index]);
		else if (nb_regular_arg==1)
			density = atof(argv[index]);
		else if (nb_regular_arg==2)
			vect_norm = atoi(argv[index]);
		
		nb_regular_arg++;
	}
	

	if(nb_regular_arg != NUMBER_ARGS)
	{
		printf("Wrong arguments !\n--help to get help\n");
		return 1;
	}
	

	//create random Choice-Free from its repetition vector norm
	printf("Generation of a random Choice-Free with : \n"\
		   "\t" "[nb_transition] = %u \n" \
		   "\t" "[density] = %lf \n" \
		   "\t" "[vect_norm] = %u \n" \
		   "\t" "[cleanExtraMem] = %s \n" \
		   "\t" "[SDF] = %s \n" \
		   , nb_transition, density, vect_norm, (cleanExtraMem)?"yes":"no", (SDF)?"yes":"no");
	
	//create Choice-Free
	srand(time(NULL));
	
	pPetri net = generateRandomChoiceFree(&real_vect_norm, nb_transition, density, vect_norm, SDF, cleanExtraMem);
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

