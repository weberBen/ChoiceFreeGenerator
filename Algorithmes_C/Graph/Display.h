#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "Array.h"
#include "Tools.h"

#define CHAR_ARRAY_SEPARATOR '|'
#define CHAR_LIST_SEPARATOR '/'
#define CHAR_DATA_SEPARATOR ';'
#define CHAR_INNER_DATA_SEPARATOR ','
#define BUFFER_SIZE 128

//debug functions
void displayGraph(pArray * list, unsigned int n);
void displaySimpeNodeArray(pArray p);//array that contains only integer
void displaySimpeNodeArray2(pArray2 p);//array that contains only integer
void displayMatrix(int a[], unsigned int num_l, unsigned int num_c);
void displayArray(double * p, unsigned int size);
void displayPetriNet(pPetri net);
void displayPetriNode(pPetriNode p, unsigned int index);
void displayPetriLink(pPetriLink p);
void displayPetriElem(pPetriElem p);
void displayPartitionSet(pPartitionSet p);

//python functions
unsigned int arrayToString(char ** output, pArray p);
unsigned int listToString(char ** output, pArray a[], unsigned int n);
int listToFile(char * filename, pArray a[], unsigned int n);
void petriWrite(pPetri net, int fileDescriptor);


//PNML function
void petriToPnmlDisplay(pPetri net, char * network_name);
void petriToPnmlFile(pPetri net, char * network_name, char* filename);

#endif
