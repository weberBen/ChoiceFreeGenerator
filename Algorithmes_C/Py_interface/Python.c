#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "Python.h"


/**********************************************************************
 * 
 * 
 * 				SOCKET COMMUNICATION WITH PYTHON FUNCTIONS
 * 
 * 
 **********************************************************************/
static graph * nonVolatileGraph = NULL;

void sendString(char * txt, char * buff, unsigned int buffer_size, unsigned int csock)
{
	/* From an array of char write into the buffer the desired text following
	 * the maximun size of the buffer.
	 * The function write one by one the character into a temporary varibale
	 * when the size match with the size of the buffer, we write all the data into it
	 * and start again.
	 * All the text will end by the end char '\0' in the buffer
	*/
	unsigned int count =0;
	char * cursor = txt;
	
	while(*cursor)
	{
		if(count==buffer_size)
		{
			//send part of the message
			sendMsg(csock, txt, sizeof(char)*buffer_size);
			
			//reset parms
			txt = cursor;
			count=0;
			//bzero(buff, buffer_size);
		}
		
		count++;
		cursor++;
	}
	
	if(count==buffer_size)//count exclude '\0'
	{
		sendMsg(csock, txt, sizeof(char)*count);
		txt=cursor;
		//wait for response
		sendMsg(csock, txt, sizeof(char)*1);
	}else if(count<buffer_size)
	{
		sendMsg(csock, txt, sizeof(char)*(count+1));//+1 for the char '\0'
	}
}

void sendNullResponse(char * buff, unsigned int buffer_size, unsigned int csock)
{
	char s[1]={'\0'};//return void graph
	sendString(s, buff, buffer_size, csock);//send the string
}

int py_establishCommunication(int PORT, int BUFFSIZE)
{
	/* Create a socket that will be used by python script to 
	 * sahre data
	*/
    char buff[BUFFSIZE];
    int ssock, csock;
    int nread;
    struct sockaddr_in client;
    socklen_t clilen = sizeof(client);
	int open;
	
    ssock = createSocket(PORT);
    fprintf(stderr,"Demmarrage du server au port %d\n", PORT);

	open = 1;
    while (open!=0)
    {
        csock = accept(ssock, (struct sockaddr *)&client, &clilen);
        if (csock < 0)
        {
            fprintf(stderr,"Erreur lors de la creation du fichier -> accept()\n");
            continue;
        }
        fprintf(stderr,"Connection etabli avec <<%s>>\n", inet_ntoa(client.sin_addr));
        
        bzero(buff, BUFFSIZE);//reset the buffer
        
        if((nread=read(csock, buff, sizeof(request)) > 0))//client send new request
        {
			request * req = (request*) buff;
			fprintf(stderr, "\tTache entrante (%d) --> ", req->task);
			
			switch(req->task)//get the corresponding task
			{
				case closeServer://close the server
				{
					fprintf(stderr, "Fermeture du server\n");
					
					open = 0;
					sendNullResponse(buff, BUFFSIZE, csock);
				}
					break;
				case f_createNonVolatileTree:
				{
					fprintf(stderr, "creation d'un arbre semi-permanent (nb_noeud=%u, max_enfant=%d)\n", req->n, req->D);
					
					//create graph that will not be discard at the end of the request
					graphFree(nonVolatileGraph);//free the previous one (if it exists)
					nonVolatileGraph = (graph *)malloc(sizeof(nonVolatileGraph));//create new one
					nonVolatileGraph->data =  buildTree(req->n, req->D);
					nonVolatileGraph->size = req->n;
					
					//send data
					char * s;
					listToString(&s,nonVolatileGraph->data, req->n);//convert the tree into a string
					
					sendString(s, buff, BUFFSIZE, csock);//send the string
					
					free(s);
				}
					break;
				case f_freeNonVolatileGraph :
				{
					fprintf(stderr, "Effacement du graphe non volatil\n");
					
					graphFree(nonVolatileGraph);
					nonVolatileGraph = NULL;
					
					sendNullResponse(buff, BUFFSIZE, csock);
				}
					break;
				case f_stronglyConnectedOnNonVolatileGraph:
				{
					fprintf(stderr, "Creation d'un graph fortement connexe à partir du graphe non volatil\n");
					
					if(nonVolatileGraph==NULL)
					{
						fprintf(stderr, "\tAttention : le graphe n'a pas encore été crée\n");
						sendNullResponse(buff, BUFFSIZE, csock);
						break;
					}
					
					stronglyConnectedGraph(nonVolatileGraph->data, nonVolatileGraph->size);
					
					char * s;
					listToString(&s,nonVolatileGraph->data, nonVolatileGraph->size);//convert the tree into a string
					sendString(s, buff, BUFFSIZE, csock);
					free(s);
					
				}
					break;
				case f_createTree:
				{	
					fprintf(stderr, "creation d'un arbre (nb_noeud=%u, max_enfant=%d)\n", req->n, req->D);
					
					pArray * tree = buildTree(req->n, req->D);//create graph
					
					char * s;
					listToString(&s,tree, req->n);//convert the tree into a string
					
					sendString(s, buff, BUFFSIZE, csock);//send the string
					
					freeList(tree, req->n);//free the memory
					free(s);
				}
					break;
				case f_createStronglyConnectedGraph:
				{
					fprintf(stderr, "creation d'un graph fortement connexe (nb_noeud=%u, max_enfant=%d)\n", req->n, req->D);
					
					pArray * tree = buildTree(req->n, req->D);//create graph
					stronglyConnectedGraph(tree, req->n);
					
					char * s;
					listToString(&s,tree, req->n);//convert the tree into a string
					sendString(s, buff, BUFFSIZE, csock);//send the string
					
					freeList(tree, req->n);//free the memory
					free(s);
				}
					break;
				
				
			}
		}
        printf("Fin de transmission\n");
        printf("----------------------------\n");
        closeSocket(csock);
    }
    
    //close server
    graphFree(nonVolatileGraph);
	nonVolatileGraph = NULL;
	
    closeSocket(ssock);
    printf("Serveur ferme\n");
    
    return 0;
}
