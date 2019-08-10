#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "Python.h"

typedef enum Tasks {t_closeServer=-1, t_buildTree = 0, t_stronglyConnectedGraph=1, t_randomGraph=2, t_free=3, t_petri=4} tasks;

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

/**********************************************************************
 * 
 * 
 * 				SOCKET COMMUNICATION WITH PYTHON FUNCTIONS
 * 
 * 
 **********************************************************************/

static pArray _list = NULL;


static char * nullResponse()
{
	char * s = (char *)malloc(sizeof(char)*1);
	s[0] = '\0';
	
	return s;
}

static void sendNullResponse(char * buff, unsigned int buffer_size, unsigned int csock)
{
	char * s = nullResponse();
	sendString(s, buff, buffer_size, csock);//send the string
	free(s);
}

static void sendResponse(request * req, char * buff, int BUFFSIZE, unsigned int csock)
{
	char * s = NULL;
	
	switch(req->task)
	{
		case t_buildTree:
		{
			printf("creation d'un arbre (nb_noeud=%u, max_enfant=%d)\n", req->n, req->D);
					
			pDirectedGraph tree = buildTree(req->n, req->D);//create graph
			wrapperAddToList(&_list, wrapperCreateNode(req->wrapperId, directedGraph_t, (void *)tree));
			
			listToString(&s,tree->links_list, req->n);//convert the tree into a strings
			
		}
			break;
		case t_stronglyConnectedGraph:
		{
			printf("Modification du graphe %d en graphe fortement connexe\n", req->wrapperId);
			
			pWrapper p = wrapperGetElem(_list, req->wrapperId);
			pDirectedGraph  graph1 = (pDirectedGraph)(p->data);
			stronglyConnectedGraph(graph1, req->isTree);
			
			listToString(&s,graph1->links_list, graph1->nb_nodes);//convert the tree into a strings
		}
			break;
		case t_randomGraph:
		{
			printf("Creation d'un graphe aleatoirement\n");
			
			pDirectedGraph graph1 = randomGraph(req->n, req->Ki, req->Ko);//create graph
			wrapperAddToList(&_list, wrapperCreateNode(req->wrapperId, directedGraph_t, (void *)graph1));
			
			listToString(&s,graph1->links_list, req->n);//convert the tree into a strings
		}
			break;
		case t_free:
		{
			printf("Suppression de l'element identifie : %d\n", req->wrapperId);
			
			wrapperRemoveFromList(&_list, req->wrapperId);
			
			s = nullResponse();
		}
			break;
		case t_petri:
		{
			printf("Conversion du graphe en réseau de pétri\n");
			
			pWrapper p = wrapperGetElem(_list, req->wrapperId);
			pDirectedGraph graph1 = (pDirectedGraph)(p->data);
			
			pPetri petriN = petriTransformation(graph1);
			wrapperAddToList(&_list, wrapperCreateNode(req->newWrapperId, petri_t, (void *)petriN));
			
			wrapperRemoveFromList(&_list, req->wrapperId);
			
			petriWrite(petriN, csock);//write petri net to the socket

			return ;
		}
			break;
		default :
		{
			printf("Demande inconnue\n");
			s = nullResponse();
		}
			break;
	}
	
	if(s==NULL)
	{
		s = nullResponse();
	}
	
	sendString(s, buff, BUFFSIZE, csock);//send the string
	free(s);
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
			tasks task = req->task;
			
			fprintf(stderr, "\tTache entrante (%d) --> ", req->task);
			
			if(task==t_closeServer)
			{
				fprintf(stderr, "Fermeture du server\n");
					
				open = 0;
				sendNullResponse(buff, BUFFSIZE, csock);
				break;
			}
			
			sendResponse(req, buff, BUFFSIZE, csock);
		}
        printf("Fin de transmission\n");
        printf("----------------------------\n");
        closeSocket(csock);
    }
    
    //close server
    
    _list = wrapperFreeList(_list);
       
    closeSocket(ssock);
    printf("Serveur ferme\n");
    
    return 0;
}
