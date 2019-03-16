//https://gist.github.com/marcom04/22860f1168330605cac3c448982b0393

#ifndef _SERVER_H
#define _SERVER_H

#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#pragma pack(1)

typedef struct payload_t {
    uint32_t id;
    uint32_t counter;
    float temp;
} payload;

#pragma pack()

typedef struct Request 
{
	int task;
	unsigned int n;
	unsigned int D;
	char * file;
} request;


enum tasks {closeServer=-1, f_createTree=0, f_createStronglyConnectedGraph=1, f_createNonVolatileTree=2, f_freeNonVolatileGraph=3, 
			f_stronglyConnectedOnNonVolatileGraph=4};

int createSocket(int port);
void closeSocket(int sock);
void sendMsg(int sock, void* msg, uint32_t msgsize);

#endif 
