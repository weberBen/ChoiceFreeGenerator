#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Rand.h"

#define FREE_VALUE 0
#define USED_VALUE -1

static int * _list = NULL;
static unsigned int _size = 0;
static int _count = 0;


void freeStaticArray()
{
	_size = 0;
	if(_list==NULL)
		return;
	
	free(_list);
	_list=NULL;
}

void allocateStaticArray(unsigned int range)
{
	freeStaticArray(_list);
	_list = (int *)malloc(sizeof(int)*range);
	assert(_list);
	_size = range;
}

void initializeStaticArray()
{
	unsigned int i;
	for(i=0; i<_size; i++)
	{
		_list[i] =  FREE_VALUE;
	}
}

void randIni(int start, int end)
{
	int range = end - start +1;
	if(range==0)
	{
		fprintf(stderr, "randIni ne peut pas prendre une taille de zeros comme entree !\n");
		exit(-1);
	}
	
	allocateStaticArray(range);
	initializeStaticArray();
	_count = range;//number of item
}

void randEnd()
{
	_count = 0;
	freeStaticArray();
}

int randArray()
{
	if(_list==NULL)
	{
		fprintf(stderr, "Impossible d'executer randArray() sans avoir initialiser l'object\n");
		exit(-1);
	}
	
	if(_count==0)
	{
		return -1;
	}
	
	unsigned int i;
	unsigned int cn;
	int val;
	unsigned int alea = 1 + rand()%_count;
	
	cn=0;
	val = -1;
	for(i=0; i<_size; i++)
	{
		if(_list[i]==FREE_VALUE)
		{
			cn++;
		}
		
		if(cn==alea)
		{
			val = i;
			_list[i] = USED_VALUE;
			break;
		}
	}
	
	_count--;
	
	return val; 
}



int getRandomInSegment(int start, int end)
{
	int m = end-start+1;
	
	if(m==0)
		return start;
	
	return start + rand()%m;
}
