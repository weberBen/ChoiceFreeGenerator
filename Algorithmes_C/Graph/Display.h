#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "Array.h"
#include "Tools.h"

#define CHAR_ARRAY_SEPARATOR '|'
#define CHAR_LIST_SEPARATOR '/'
#define CHAR_DATA_SEPARATOR ';'

void displayGraph(pArray * list, unsigned int n);
void displaySimpeNodeArray(pArray p);
void displaySimpeNodeArray2(pArray2 p);
void displayMatrix(int a[], unsigned int num_l, unsigned int num_c);
void displayArray(double * p, unsigned int size);


unsigned int arrayToString(char ** output, pArray p);
unsigned int listToString(char ** output, pArray a[], unsigned int n);
unsigned int petriToString(char ** output, pPetri p);
int listToFile(char * filename, pArray a[], unsigned int n);

void displayPetriNet(pPetri net);

#endif
