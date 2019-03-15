#ifndef _PYTHON_H
#define _PYTHON_H

#include "GraphGenerator.h"
#include "Tools.h"
#include "Server.h"

int startPyFunctions(int argc, char **argv);

void py_createTree(unsigned int n, unsigned int D, char * filename);
void py_createStronglyConnectedGraph(unsigned int n, unsigned int D, char * filename);

int py_establishCommunication(int PORT, int BUFFERSIZE);

#endif 

