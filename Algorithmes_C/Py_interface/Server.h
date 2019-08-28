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

int createSocket(int port);
void closeSocket(int sock);
void sendMsg(int sock, void* msg, uint32_t msgsize);
void sendString(char * txt, char * buff, unsigned int buffer_size, unsigned int csock);

#endif 
