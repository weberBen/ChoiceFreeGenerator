#ifndef _ARRAY_H
#define _ARRAY_H

typedef struct Array * pArray;

typedef struct Array
{
    unsigned int data;
    struct Array * next;
} array;

pArray createNode(unsigned int data);
pArray add(pArray l, unsigned int data);

void initializeArray(pArray a[], unsigned int n);

void freeArray(pArray p);
void freeList(pArray a[], unsigned int n);

#endif 
