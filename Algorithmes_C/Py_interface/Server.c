#include "Server.h"

int createSocket(int port)
{
    int sock;
    struct sockaddr_in server;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("ERROR: Socket creation failed\n");
        exit(1);
    }
    printf("Socket created at port :%d\n", port);

    bzero((char *) &server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("ERROR: Bind failed\n");
        exit(1);
    }
    printf("Bind done\n");

    listen(sock , 3);

    return sock;
}

void closeSocket(int sock)
{
    close(sock);
    return;
}

void sendMsg(int sock, void* msg, uint32_t msgsize)
{
    if (write(sock, msg, msgsize) < 0)
    {
        printf("Can't send message.\n");
        closeSocket(sock);
        exit(1);
    }
    printf("Message sent (%d bytes).\n", msgsize);
    return;
}

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
