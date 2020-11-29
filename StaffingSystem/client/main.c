#include <unistd.h>
#include "client.h"

int main(int argc, char *argv[])
{
	int sockfd = -1;
	/*
	//测试服务器与客户端的连通
	sockfd = tcp_client_create(argc,argv);
	tcp_client_test(sockfd);
	close(sockfd);
	sockfd = -1;
	*/
	sockfd = tcp_client_create(argc,argv);

	start(sockfd);
	
    close(sockfd);
	sockfd = -1;
    return 0;
}

