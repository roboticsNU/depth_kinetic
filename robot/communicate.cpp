#include <sys/types.h> 
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <errno.h> 
#include <WinSock2.h>
#include <Windows.h> 
#include <Ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

bool volatile canstart = false;
extern bool volatile finished;
bool volatile commready = false;

int communicate() {
	while (!canstart);

    char * ptrEnd;
    int num;
    int sockfd = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr;

    // LAN

    //          char ipaddr[100] = "10.1.71.141";
    //WiFi

    char ipaddr[100] = "10.1.71.125";

    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if(inet_pton(AF_INET, ipaddr, &serv_addr.sin_addr)<=0) {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\n Error : Connect Failed \n");
        return 1;
    } else {
		commready = true;
		char buf[1];
		buf[0] = 0;
		printf("\n Connected reading now \n");
        recv(sockfd, buf, 1, MSG_WAITALL);
		printf("\n DONE \n");
        finished = true;
	}

	return 0;
}