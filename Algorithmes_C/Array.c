#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Array.h"

pArray createNode(unsigned int data)
{
    pArray node = (pArray)malloc(sizeof(array));
    assert(node);

    node->data = data;
    node->next = NULL;

    return node;
}

pArray add(pArray l, unsigned int data)
{
    pArray node = createNode(data);
    node->next = l;

    return node;
}

void initializeArray(pArray a[], unsigned int n)
{
	unsigned int i;
	
	for(i=0; i<n; i++)
	{
		a[i] = NULL;
	}
}

void freeArray(pArray p)
{
	pArray cursor = p;
	
	while(cursor!=NULL)
	{
		p = cursor;
		cursor = cursor->next;
		free(p);
	}
}

void freeList(pArray a[], unsigned int n)
{
	if(a==NULL)
		return;
	
	unsigned int i;
	
	for(i=0; i<n; i++)
	{
		freeArray(a[i]);
	}
	
	free(a);
}

