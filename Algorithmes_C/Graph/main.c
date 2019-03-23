#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "GraphGenerator.h"
#include "Tools.h"
#include "Rand.h"

void randomMatrix(int * m, unsigned int num_l, unsigned int num_c)
{
	unsigned int i, j;
	for(i=0;i<num_l; i++)
	{
		for(j=0;j<num_c; j++)
		{
			m[i*num_c+j] = rand()%10;
		}
	}
}

int main(int argc, char ** argv)
{
	
	//test functions
	
	//srand(time(NULL));
	
	unsigned int num_pl = 5;
	unsigned int num_tr = 8;
	
	int * places = (int *)malloc(sizeof(int)*num_pl);
	assert(places);
	randomMatrix(places, 1, num_pl);
	int * trans = (int *)malloc(sizeof(int)*(num_tr*num_pl));
	assert(trans);
	randomMatrix(trans, num_tr, num_pl);
	
	displayMatrix(places, 1, num_pl);
	fprintf(stderr, "****\n");
	displayMatrix(trans, num_tr, num_pl);
	fprintf(stderr,"\n");
	
	
	pPetri p = petriCreateNode();
	p->places=places;
	p->num_pl = num_pl;
	p->trans=trans;
	p->num_tr = num_tr;
	
	char * s;
	petriToString(&s, p);
	fprintf(stderr, "val=%s\n", s);
	
	free(s);
	petriFree(p);
	return 0;
}


