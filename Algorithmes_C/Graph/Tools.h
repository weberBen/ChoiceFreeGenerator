#ifndef _TOOLS_H
#define _TOOLS_H

#include "Array.h"

void displayGraph(pArray * list, unsigned int n);
void displaySimpeNodeArray(pArray p);
 void displayMatrix(int a[], unsigned int num_l, unsigned int num_c);


unsigned int arrayToString(char ** output, pArray p);
unsigned int listToString(char ** output, pArray a[], unsigned int n);
unsigned int petriToString(char ** output, pPetri p);


int listToFile(char * filename, pArray a[], unsigned int n);

unsigned int getNumberOfDigit(int num);



#endif 
