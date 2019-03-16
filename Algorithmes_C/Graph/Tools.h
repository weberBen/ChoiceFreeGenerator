#ifndef _TOOLS_H
#define _TOOLS_H

#define ARRAY_CHAR_SEPARATOR '-'
#define LIST_CHAR_SEPARATOR '|'

#include "Array.h"

void displayGraph(pArray * list, unsigned int n);
void displaySimpeNodeArray(pArray p);

unsigned int arrayToString(char ** output, pArray p);
unsigned int listToString(char ** output, pArray a[], unsigned int n);

int listToFile(char * filename, pArray a[], unsigned int n);

unsigned int getNumberOfDigit(int num);

#endif 
