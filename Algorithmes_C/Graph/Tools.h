#ifndef _TOOLS_H
#define _TOOLS_H

#include "Array.h"

#define max(a,b) \
({ __typeof__ (a) _a = (a); \
   __typeof__ (b) _b = (b); \
 _a > _b ? _a : _b; })
     
void displayGraph(pArray * list, unsigned int n);
void displaySimpeNodeArray(pArray p);
void displayMatrix(int a[], unsigned int num_l, unsigned int num_c);
void displayArray(double * p, unsigned int size);


double * copyFloatArray(double p[], unsigned int size);
void initializeFloatArray(double p[], unsigned int size, double value);
void initializeIntArray(int p[], unsigned int size, int value);
void initializeList(void * p[], unsigned int n);

unsigned int arrayToString(char ** output, pArray p);
unsigned int listToString(char ** output, pArray a[], unsigned int n);
unsigned int petriToString(char ** output, pPetri p);


int listToFile(char * filename, pArray a[], unsigned int n);

unsigned int getNumberOfDigit(int num);


double * getCumulativeProba(double * proba, unsigned int size);
void updateCumulativeProba(double * cumulProba,
						double * proba, unsigned int size, unsigned int index);
int getIndexCumulativeProba(double * cumulProba, unsigned int size, double valueToFind);

#endif 
