#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>
#include <limits.h>
#include <math.h>

#include "choiceFreeGeneratorDev.h"

#define NB_SIMULATION 1
#define SIZE 100
#define NORM 4950
#define MIN 1
#define MAX 50
#define FUNCTION 4

void initializeuIntArray(unsigned int * array, unsigned int size, unsigned int val)
{
    int i;
    for(i=0; i<size; i++)
    {
        array[i] = val;
    }
}

int getRandomInSegment(int start, int end)
{
	int m = end-start+1;
	
	if(m==0)
		return start;
	
	return start + rand()%m;
}


unsigned int * randomFixedSum1(unsigned int n, unsigned int sum)
{
	unsigned int * output = (unsigned int *)malloc(sizeof(unsigned int)*n);
	assert(output);
	initializeuIntArray(output, n, 1);//set to one all the value
	
	int r;
	
	sum-=n;
	while(sum>=0)
	{
		r = getRandomInSegment(0, n-1);
		output[r]++;
		sum--;
	}
	
	return output;
}



unsigned int * randomFixedSum2(unsigned int n, unsigned int sum)
{
	unsigned int * output = (unsigned int *)malloc(sizeof(unsigned int)*n);
	assert(output);

	unsigned int i;
	unsigned int count =0;

	for(i=0; i<n; i++)
	{
		output[i]= 1 + rand()%sum;
		count+=output[i];
	}

	double factor = ((float)sum)/((float)count);

	for(i=0; i<n; i++)
	{
		output[i]= ceil(output[i]*factor);
	}

	return output;
}

unsigned int * randomFixedSum3(unsigned int n, unsigned int sum){
	
	unsigned int * output = (unsigned int *)malloc(sizeof(unsigned int)*n);
	assert(output);
	
	unsigned int i;
	unsigned int count = 0;
	
	for(i=0; i<n; i++){
		output[i] = 1 + rand()%(sum-n+1);
		count+=output[i];
	}
	
	double factor = ((float)sum)/((float)count);
	
    for(i=0; i<n; i++){
		if(i%2)
		    output[i]= ceil(output[i]*factor) + 1;
		else
		    output[i]= floor(output[i]*factor) + 1;

	}
	
	return output;
}


unsigned int * randomFixedSum4(unsigned int n, unsigned int sum, unsigned int min, unsigned int max)
{
	
	unsigned int * output = (unsigned int *)malloc(sizeof(unsigned int)*n);
	assert(output);
	

	unsigned int i;
	int odd;

	double init_val = 1.*sum/n;
	if(init_val<min)
		init_val=min;

	
	odd=0;
	for(i=0; i<n; i++)
	{
		if(!odd)
		{
			output[i] = ceil(init_val);
			odd=1;
		}else
		{
			output[i] = floor(init_val);
			odd=0;
		}
		
	}


	unsigned int val;
	unsigned int index;

	for(i=0; i<n; i++)
	{
		index = rand()%n;

		if(output[i]==min)
			continue;
		
		if(output[index]>=0.7*max)
			continue;
		

		val = rand()%(output[i]-min);
		
		if(output[index] + val>max)
			val = max - output[index];

		output[index]+=val;
		output[i]-=val;
	}
	
	return output;
}

unsigned int maxArray(unsigned int * array, unsigned int size)
{
    int i;
    unsigned int max = array[0];

    for(i=1; i<size; i++)
    {
        if(array[i]>max)
            max = array[i];
    }

    return max;
}


int main()
{
	srand(time(NULL));
    
    unsigned int * avg = (unsigned int *)malloc(sizeof(unsigned int)*NORM);
    unsigned int * tmp;
    double avg_sum, tmp_sum;
    
    int k, i;

    initializeuIntArray(avg, NORM, 0);
    avg_sum = 0;
    for(k=0; k<NB_SIMULATION; k++)
    {
        tmp_sum = 0;
        if(FUNCTION==1)
           tmp = randomFixedSum1(SIZE, NORM);
        else if(FUNCTION==2)
            tmp = randomFixedSum2(SIZE, NORM);
        else if(FUNCTION==3)
            tmp = randomFixedSum3(SIZE, NORM);
		else	
			tmp = randomFixedSum4(SIZE, NORM, MIN, MAX);
		

        for(i=0; i<SIZE; i++)
        {
            avg[tmp[i]]++;
            tmp_sum+=tmp[i];
        }

        avg_sum+=tmp_sum;
    }
	
    for(i=0; i<NORM; i++)
    {
        if(avg[i]==0)
            continue;
        fprintf(stderr, "( %u , %lf )\n", i, 1.*avg[i]/NB_SIMULATION);

    }

    fprintf(stderr, "avg_real_sum=%lf  | desired_sum=%u\n", 1.*avg_sum/NB_SIMULATION, NORM);

	free(avg);
	return 0;
}
