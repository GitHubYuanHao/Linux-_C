#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <pthread.h> 

#include "server.h"

int main(int argc, char *argv[])
{
    int sockfd = -1;
	int datafd = -1;
	pthread_t pid;
	
	sockfd = tcp_server_create(argc, argv);
	
	while ( 1 )
	{
		datafd = tcp_accept_datafd(sockfd);
		if (datafd < 0)
		{
			perror("fail to accept");
			exit(-1);
		}
		pthread_create(&pid,NULL,start,(void *)&datafd);
		pthread_detach(pid);
	}
	
	close(sockfd);
	sockfd = -1;
    return 0;
}