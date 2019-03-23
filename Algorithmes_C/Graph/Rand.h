#ifndef _RAND_H
#define _RAND_H

#include "Array.h"

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
		printf("unique value between 0 and 4 : %d\n", res);
	}while(res!=-1);

	randEnd(r1);
 * 
 * 
 * 
	unsigned int r1 = randIni(0,4);
	unsigned int r2 = randIni(12,24);		

	int res1, res2;
	do
	{
		res1 = randArray(r1);
		res2 = randArray(r2);
		printf("\nunique value between 0 and 4 : %d\n", res1);
		printf("unique value between 12 and 24 : %d\n", res2);
	}while(res1!=-1 && res2!=-1);

	randEnd(r1); //or randEnd(r2), and then, randEnd(r1)
	randEnd(r2);
 * 
 *  
*/



/**********************************************************************
 * 				get random value between two integer
 **********************************************************************/
int getRandomInSegment(int start, int end);

#endif
