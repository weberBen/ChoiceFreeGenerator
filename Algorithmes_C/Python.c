#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "Python.h"

int startPyFunctions(int argc, char ** argv)
{
	int opt; 
    unsigned int function = -1;
    
    while((opt = getopt(argc, argv, "f:")) != -1)  //-f requeries paramters so f:
    {  
        switch(opt)  
        {  
            case 'f': 
            {
				char names[][50] ={"py_createTree", "py_createStronglyConnectedGraph", ""};
				unsigned int i;
				
				i=0;
				while(names[i][0]!='\0' && strcmp(optarg,names[i]))
				{
					i++;
				}
				
				if(names[0][0]=='\0')
				{
					fprintf(stderr, "Fonction <<%s>> introuvable !", optarg);
					return -1;
				}
				function = i;
			} 
                break;  
            case ':':  
                printf("option needs a value\n");  
                break;  
            case '?':  
                printf("option %c inconnu\n", optopt); //optarg
                break;  
        }  
    }  
      
      
    if(function==-1)
    {
		return -1;
	}
	
	switch(function)
	{
		case 0:
		{
			if(argc-optind<3)
			{
				printf("Nombre d'arguments fourni incorrects !\n");
				return -1;
			}
			py_createTree(atoi(argv[optind]), atoi(argv[optind+1]), argv[optind+2]);
		}
			break;
		case 1:
		{
			if(argc-optind<3)
			{
				printf("Nombre d'arguments fourni incorrects !\n");
				return -1;
			}
			py_createStronglyConnectedGraph(atoi(argv[optind]), atoi(argv[optind+1]), argv[optind+2]);
		}
			break;
		
	}
    /*
    // optind is for the extra arguments 
    // which are not parsed 
    for(; optind < argc; optind++)
    {      
        printf("extra arguments: %s\n", argv[optind]);  
    }*/ 
    
    return 0;
}


void py_createTree(unsigned int n, unsigned int D, char * filename)
{
	pArray * tree = buildTree(n, D);
	listToFile(filename, tree, n);
	freeList(tree, n);
}

void py_createStronglyConnectedGraph(unsigned int n, unsigned int D, char * filename)
{
	pArray * tree = buildTree(n, D);
	stronglyConnectedGraph(tree, n);
	listToFile(filename, tree, n);
	freeList(tree, n);
}
