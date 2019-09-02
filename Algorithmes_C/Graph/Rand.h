#ifndef _RAND_H
#define _RAND_H

#include "Array.h"
#include "Tools.h"

/**********************************************************************
 * 				get unique random value in an array
 **********************************************************************/
 
unsigned int randIni(int start, int end);
int randArray(unsigned int id);
void randEnd(unsigned int id);
/* Example of use :
 * 
	unsigned int r1 = randIni(0,4);
	
	int res;
	do
	{
		res = randArray(r1);
		printf("unique value between 0 and 3 : %d\n", res);
	}while(res!=-1);

	randEnd(r1);
 
	
	r1 = randIni(0,4);
	unsigned int r2 = randIni(12,24);		

	int res1, res2;
	do
	{
		res1 = randArray(r1);
		res2 = randArray(r2);
		printf("\nunique value between 0 and 3 : %d\n", res1);
		printf("unique value between 12 and 23 : %d\n", res2);
	}while(res1!=-1 && res2!=-1);

	randEnd(r1); //or randEnd(r2), and then, randEnd(r1)
	randEnd(r2);
 * 
 *  
*/

/**********************************************************************
 * 		get random value from an array filled with probability value
 **********************************************************************/

unsigned int randCumulProbaIni(double * proba, unsigned int size);
void randCumulProbaEnd(unsigned int id);
int randCumulProba(unsigned int id);
void randCumulProbaUpdate(unsigned int id, unsigned int index);
void randCumulProbaChangeData(unsigned int id, double * proba, unsigned int size);


/**********************************************************************
 * 				get random value between two integer
 **********************************************************************/
int getRandomInSegment(int start, int end);

/**********************************************************************
 * 				get random values with fixed sum
 **********************************************************************/

unsigned int * randomFixedSum(unsigned int * real_sum, unsigned int n, unsigned int sum);
unsigned int * randomFixedSumBounded(unsigned int * real_sum, unsigned int n, unsigned int sum, unsigned int min, unsigned int max);

/**********************************************************************
 * 				random tools function
 **********************************************************************/
double randf(double start, double end);


/**********************************************************************
 * 				random partition of set
 **********************************************************************/

unsigned int * randomSubSet(unsigned int tot_nb_elem_set, unsigned int nb_elem_partition);//return index of element inside the new partition
pPartitionSet randomPartitions(unsigned int tot_nb_elem_set, unsigned int nb_partitions);


#endif
