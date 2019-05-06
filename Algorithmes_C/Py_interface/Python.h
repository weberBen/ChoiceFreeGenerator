#ifndef _PYTHON_H
#define _PYTHON_H

#include "GraphGenerator.h"
#include "Tools.h"
#include "Server.h"

typedef struct Request 
{
	int task;
	unsigned int n;
	unsigned int D;
	unsigned int wrapperId;
	unsigned int newWrapperId;
	int Ki;
	int Ko;
	int isTree;
} request;

int py_establishCommunication(int PORT, int BUFFERSIZE);
char * getResponse(request * req);
typedef enum Tasks {t_closeServer=-1, t_buildTree = 0, t_stronglyConnectedGraph=1, t_randomGraph=2, t_free=3, t_petri=4} tasks;
#endif 

