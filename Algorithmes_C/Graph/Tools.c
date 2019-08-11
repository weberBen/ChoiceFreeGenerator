#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "Tools.h"

 
 /**********************************************************************
 * 
 * 
 * 						ARRAY FUNCTION
 * 
 **********************************************************************/
 double * copyFloatArray(double p[], unsigned int size)
 {
	 double * output = (double *)malloc(sizeof(double)*size);
	 assert(output);
	 
	 unsigned int i;
	 for(i=0; i<size; i++)
	 {
		 output[i]=p[i];
	 }
	 
	 return output;
 }
 
 
 void initializeFloatArray(double p[], unsigned int size, double value)
 {
	 double * output = (double *)malloc(sizeof(double)*size);
	 assert(output);
	 
	 unsigned int i;
	 for(i=0; i<size; i++)
	 {
		 output[i]=value;
	 }
 }
 
 void initializeIntArray(int p[], unsigned int size, int value)
 {
	unsigned int i;
	for(i=0; i<size; i++)
	{
		p[i] =  value;
	}
 }
 
 void initializeList(void * p[], unsigned int size)
 {
	unsigned int i;
	
	for(i=0; i<size; i++)
	{
		p[i] = NULL;
	}
 }
 

 
/**********************************************************************
 * 
 * 
 * 						PROBABILITY FUNCTIONS
 * 
 **********************************************************************/
 
double * getCumulativeProba(double * proba, unsigned int size)
{
	/* From an array of probability return the associate cumulative probability array*/
	double * cumulProba = (double *)malloc(sizeof(double)*size);
	assert(cumulProba);
	updateCumulativeProba(cumulProba, proba, size, 0);
	
	return cumulProba;
}

void updateCumulativeProba(double * cumulProba,
						double * proba, unsigned int size, unsigned int index)
{
	/* From the given index update the cumulative array*/
	 unsigned int i;
	 if(index==0)
	 {
		cumulProba[0] = proba[0];
		index = 1;
	 }

	 for(i=index; i<size; i++)
	 {
		 cumulProba[i] = cumulProba[i-1] + proba[i];
	 }
}


int getIndexCumulativeProba(double * cumulProba, unsigned int size, double valueToFind)
{
	/* If the cumulative probability array looks like [a(1), a(2), a(3), ... a(n)]
	 * Let's suppose that a1 != 0, then we used the following segments for probability
	 * [0,a(1)[ - [a(1),a(2)[ - [a(2),a(3)[ - ... - [a(n-1), a(n)[
	 * 
	 * For a given value that will be in [0,a(1)] the return index is 0, for [a(1),a(2)[ the index is 1
	 * 
	 * In general for a cumulative array (a(i)) i in [1, n] with a(0)=0, then segment a value in [a(i),a(i+1)[
	 * will return an index of i if a(i+1)!=0
	 * Else (a(i+1)=0), then the return index is the first p as (p-1)>i+1 and a(p)!=0
	 * 
	 * For example with [0,0,5,0,5,0,0,0]
	 * The segments are [0,5[ - [5-10[
	 * Then a value in [0,5[=[a(0),a(3)[ will give 2 and between [5,10[=[a(3)-a(5)[ it will give 4
	 * 
	 * Another example with [5,10,15,20]
	 * The segments are [0,5[ - [5,10[ - [10,15[ - [15,20[
	 * Then a value in [0,5[=[a(0),a(1)[ will give 0 and between [5,10[=[a(1),a(2)[ it will give 1
	*/
	 
	if(valueToFind>=cumulProba[size-1])
		return -1;
	
	unsigned int i;
	
	i=0;
	while(i<size && cumulProba[i]<=valueToFind)
	{
		i++;
	}
	
	return i;
}



/**********************************************************************
 * 
 * 
 * 						ARITHMETIC FUNCTIONS
 * 
 **********************************************************************/

unsigned int gcd(unsigned int x, unsigned int y)
{
	/* Return the Greatest Common Divisor of two integer , or -1 if integers are nul*/
	if(x==0 || y==0)
		return -1;
	
    unsigned int wk;
    if(x<y){ wk=x;x=y;y=wk; }
    while(y)
	{
        wk = x%y;
        x=y;
        y=wk;
    }
    return x;
}

int gcd_array(unsigned int * input, unsigned int size)
{
	/* Return the Greatest Common Divisor for an integer array, or -1 if there is no element in the array*/
	if(size==0)
		return -1;
	
	int i;
	unsigned int gcd_val = input[0];
	for(i=1; i<size; i++)
	{
		gcd_val = gcd(gcd_val, input[i]);
		if(gcd_val==-1)
			return -1;
	}

	return gcd_val;
}

int lcm(unsigned int x, unsigned int y)
{
	int gcd_val = gcd(x, y);
	if(gcd_val==-1)
		return -1;
	
	return (x*y)/gcd_val;
}

int lcm_array(unsigned int * input, unsigned int size) 
{ 
    // Initialize result 
    unsigned int lcm_val = input[0]; 
	int gcd_val;
    int i;
    for (i=1; i < size; i++) 
	{
		gcd_val = gcd(input[i], lcm_val);
		if(gcd_val==-1)
			return -1;
        lcm_val = ((input[i] * lcm_val)) / gcd_val; 
	}
  
    return lcm_val; 
} 


