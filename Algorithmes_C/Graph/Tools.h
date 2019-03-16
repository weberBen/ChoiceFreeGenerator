#ifndef _TOOLS_H
#define _TOOLS_H

#define ARRAY_CHAR_SEPARATOR '-'
#define LIST_CHAR_SEPARATOR '|'

#include "Array.h"

void displayList(pArray * list, unsigned int n);
void displayArray(pArray p);

unsigned int arrayToString(char ** output, pArray p);
unsigned int listToString(char ** output, pArray a[], unsigned int n);

int listToFile(char * filename, pArray a[], unsigned int n);

unsigned int getNumberOfDigit(int num);

#endif 
