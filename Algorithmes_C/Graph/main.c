#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "GraphGenerator.h"
#include "Tools.h"
#include "Rand.h"


int main(int argc, char ** argv)
{
	
	//test functions
	
	//srand(time(NULL));
	
	unsigned int n =1 + rand()%1000;
	unsigned int sum = n + rand()%(3*n-n+1);
	unsigned int *r = randomFixedSum(n, sum);
	unsigned int i;
	int count=0;
	
	for(i=0; i<n; i++)
	{
		fprintf(stderr, "%d\n", r[i]);
		//fprintf(stderr, "res%d\n", r[i]);
		count+=r[i];
	}
	fprintf(stderr, "(n=%u, fixedSum=%d, sum=%d)\n", n, sum, count);
	
	free(r);
	
	return 0;
}


