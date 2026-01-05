#include "sockets.h"
#include <stdio.h>

void initSocket ( int* udp_sock , struct sockaddr_in* addr, in_addr_t recv_addr ) {

    *udp_sock = socket(AF_INET, SOCK_DGRAM, 0);

	if(*udp_sock < 0){
		printf("Can't create socket\n");
	}
	
	addr->sin_family = AF_INET;
	
	/* inet_addr: Convert Internet host address from numbers-and-dots notation in CP
	into binary data in network byte order.  */
	addr->sin_addr.s_addr = recv_addr; //receiver IP
	
	/*
	FROM THE MAN PAGE
	The htonl() function converts the unsigned integer hostlong from host byte order to network byte order.
	The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.
	The ntohl() function converts the unsigned integer netlong from network byte order to host byte order.
	The ntohs() function converts the unsigned short integer netshort from network byte order to host byte order.
	On  the  i386 the host byte order is Least Significant Byte first, whereas the network byte order, as used on the Internet, is Most Significant Byte
	first.
	*/

	addr->sin_port = htons(5000);

}

void sendPacket(int* udp_sock, const char* message, struct sockaddr_in* addr){
    if(sendto(*udp_sock, message, strlen(message), 0, (struct sockaddr*) addr, sizeof(*addr)) < 0) 
        printf("Failed to send message. \n");
}