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

3 Jan 2026
Made a simple 3D renderer from Tsoding's video

4 Jan 2026 
The renderer now outputs bitmaps that can be loaded into an OLED
*/

#include <stdio.h>
#include "sockets.h"

#define FREQ_MS 500

const char* message = "test";
clock_t dt = 0;

int udp_sock = -1;	
struct sockaddr_in addr;

int main(){

	initSocket( &udp_sock, &addr, inet_addr("192.168.1.10"));

	clock_t time_i = clock(); //current time, ini

	while(1){
		dt = (clock() - time_i) * 1000 / CLOCKS_PER_SEC; //The value returned is the CPU time used so far as a clock_t; to get the number  of  seconds  used,  divide by CLOCKS_PER_SEC

		if(dt > FREQ_MS){
			
			sendPacket(&udp_sock, message, &addr);
			
			time_i = clock();
			printf("TIMESTAMP: %.1f: Sending msg -> \"%s\" to %d\n", (float)(time_i)*1000/CLOCKS_PER_SEC,message, addr.sin_addr.s_addr);
			
		}
	}

	return 0;
}
