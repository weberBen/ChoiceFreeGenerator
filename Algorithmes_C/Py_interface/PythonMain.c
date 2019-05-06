#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>

#include "GraphGenerator.h"
#include "Tools.h"
#include "Python.h"
#include "Server.h"


int main(int argc, char ** argv)
{
	if(argc<3)
	{
		fprintf(stderr, "La fonction prend deux arguments <<PORT>> <<BUFFERSIZE>>\nOu : \tPORT est le port de communication du server\n\tBUFFERSIZE est la taille du buffer (<=%d)\n", INT_MAX);
		return -1;
	}
	
	int PORT = atoi(argv[1]);
    int BUFFERSIZE = atoi(argv[2]);
    
    if(BUFFERSIZE>INT_MAX)
    {
		fprintf(stderr, "La taille maximale du buffer permise est %d", INT_MAX);
		return -1;
	}
    
    srand(time(NULL));
    //start server
	
	py_establishCommunication(PORT, BUFFERSIZE);
	
	/*request req;
	req.task = t_randomGraph;
	req.n = 10;
	req.Ki=3;
	req.Ko=2;
	req.wrapperId = 1;
	
	char * s = getResponse(&req);
	fprintf(stderr, "res=%s\n", s);
	free(s);*/
	
	return 0;
}
