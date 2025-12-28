/*
26 Dec 2025
Trying to make UDP work with ESP32.
The objective is to learn the ins and outs of UDP and how it works and 
eventually send and receive telemetry data from an ESP32 using a 
custom protocol thingy. 
The whole thing is to be done without any help from an LLM.
Read man pages and online articles and figure it out.

27 Dec 2025
Learned how to create Makefiles.
Studied how UDP works.
Got UDP send working.
Implemented the feature to set frequency.

28 Dec 2025
Got string transfer working between ESP and PC
I also have to make Wifi work without the example snippet
After that I have two option: 1. Image on OLED;  2. Telemetry
Image does sound cooler
*/

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

#define FREQ_MS 500

struct sockaddr_in addr;
const char* message = "test";
clock_t dt = 0;

int main(){
	
	int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(udp_sock < 0){
		printf("Can't create socket\n");
	}
	
	addr.sin_family = AF_INET;
	
	/* inet_addr: Convert Internet host address from numbers-and-dots notation in CP
	into binary data in network byte order.  */
	addr.sin_addr.s_addr = inet_addr("192.168.1.70"); //receiver IP
	
	/*
	FROM THE MAN PAGE
	The htonl() function converts the unsigned integer hostlong from host byte order to network byte order.
	The htons() function converts the unsigned short integer hostshort from host byte order to network byte order.
	The ntohl() function converts the unsigned integer netlong from network byte order to host byte order.
	The ntohs() function converts the unsigned short integer netshort from network byte order to host byte order.
	On  the  i386 the host byte order is Least Significant Byte first, whereas the network byte order, as used on the Internet, is Most Significant Byte
	first.
	*/
	addr.sin_port = htons(5000);

	/*
	FROM THE MAN PAGE
		The value returned is the CPU time used so far as a clock_t; to get the
       number  of  seconds  used,  divide by CLOCKS_PER_SEC.  If the processor
       time used is not available or its  value  cannot  be  represented,  the
       function returns the value (clock_t) -1.

	*/
	clock_t time_i = clock(); //current time, ini

	while(1){
		dt = (clock() - time_i)*1000/CLOCKS_PER_SEC;

		if(dt > FREQ_MS){
			if(sendto(udp_sock, message, strlen(message), 0, (struct sockaddr*) &addr, sizeof(addr)) < 0) printf("Failed to send message. \n");
			
			time_i = clock();
			printf("TIMESTAMP: %.1f: Sending msg -> \"%s\" to %d\n", (float)(time_i)*1000/CLOCKS_PER_SEC,message, addr.sin_addr.s_addr);
			
		}
	}

	return 0;
}
