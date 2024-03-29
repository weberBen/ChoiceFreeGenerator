#ifndef _TOOLS_H
#define _TOOLS_H

#include <math.h>


double * copyFloatArray(double p[], unsigned int size);
void initializeFloatArray(double p[], unsigned int size, double value);
void initializeIntArray(int p[], unsigned int size, int value);
 void initializeUintArray(unsigned int p[], unsigned int size, unsigned int value);
void initializeList(void * p[], unsigned int n);
void insertInSortedArray(unsigned int * input, unsigned int size, unsigned int elem);

unsigned int getNumberOfDigit(int num);


double * getCumulativeProba(double * proba, unsigned int size);
void updateCumulativeProba(double * cumulProba,
						double * proba, unsigned int size, unsigned int index);
int getIndexCumulativeProba(double * cumulProba, unsigned int size, double valueToFind);


unsigned int gcd(unsigned int x, unsigned int y);
int gcd_array(unsigned int * input, unsigned int size);
int lcm(unsigned int x, unsigned int y);
int lcm_array(unsigned int * input, unsigned int size);
unsigned int min(unsigned int a, unsigned int b);
unsigned int max(unsigned int a, unsigned int b);
unsigned int * sortUintArray(unsigned int * input, unsigned int size);
void insertInUintSortedArray(unsigned int * input, unsigned int size, unsigned int nb_elem, unsigned int elem);

#endif 
