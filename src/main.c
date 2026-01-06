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

6 Jan 2026
Refactored code
Working stream with Arduino IDE code !!!

*/

#include <stdio.h>
#include "sockets.h"
#include "renderer.h"

#define FREQ_MS 1

const char* message = "test";
clock_t dt = 0;

int udp_sock = -1;	
struct sockaddr_in addr;

int main(){
	// SDL
	initSDL2();
	
	SDL_SetRenderTarget(renderer, oledTarget);
	SDL_RenderClear(renderer);
	
	struct point points[] = {
		createPoint(0.5,  0.5 ,  2.0, 5),
		createPoint(-0.5, 0.5 ,  2.0, 5),
		createPoint(0.5,  -0.5,  2.0, 5),
		createPoint(-0.5, -0.5,  2.0, 5),
		
		createPoint(0.5,  0.5 ,  3.0, 5),
		createPoint(-0.5, 0.5 ,  3.0, 5),
		createPoint(0.5,  -0.5,  3.0, 5),
		createPoint(-0.5, -0.5,  3.0, 5),
	};

	int edges[][2] = {
		{0, 1}, {1, 3}, {3, 2}, {2, 0},
		{4, 5}, {5, 7}, {7, 6}, {6, 4},
		{0, 4}, {1, 5}, {2, 6}, {3, 7}
	};

	//SOCKETS

	initSocket( &udp_sock, &addr, inet_addr("192.168.1.38"));

	clock_t time_i = clock(); //current time, ini
	//value returned is the CPU time used so far as a clock_t; to get the number  of  seconds  used,  divide by CLOCKS_PER_SEC


	while(running){
		while(SDL_PollEvent(&ev) != 0){
			switch(ev.type){
				case SDL_QUIT:
				running = false;
				break;
			}
		}
		memset(bmap, 0, sizeof(bmap)); //clear bitmap each frame

		SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
		SDL_RenderClear( renderer );
		
		SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
		
		dz += 0.0001*(1000/FPS);
		
		
		for(int i = 0; i < (int) (sizeof(points)/sizeof(struct point)); i++){
			updatePoint(rotatePoint(&points[i], angle));
		}

		for(int i = 0; i < 12; i++){
			int p1 = edges[i][0];
			int p2 = edges[i][1];
			
			int x1 = points[p1].r.x + points[p1].r.w / 2;
			int y1 = points[p1].r.y + points[p1].r.h / 2;
			int x2 = points[p2].r.x + points[p2].r.w / 2;
			int y2 = points[p2].r.y + points[p2].r.h / 2;
			
			SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
			// SDL_RenderFillRect(renderer, &points[i].r);
		}

		SDL_RenderReadPixels(
			renderer,
			NULL,
			SDL_PIXELFORMAT_ARGB8888,
			pixels,
			128 * 4
		);

		for (int i = 0; i < 128 * 64; i++) {
			uint32_t p = pixels[i];
			if (((p >> 16) & 0xFF) > 128) {  // checking only one of the channels, as its either black or white
				bmap[i / 8] |= (1 << (7 - (i % 8))); //check notes!
			}
		}
		
		angle+=2.0*3.14*(1000.0/FPS)*0.0001;

		SDL_SetRenderTarget(renderer, NULL); //set renderer back to window
		
		SDL_RenderPresent(renderer);

		////////////////SOCKETS//////////////////

		dt = (clock() - time_i) * 1000 / CLOCKS_PER_SEC; 

		if(dt > FREQ_MS){

			sendPacket(&udp_sock, bmap, sizeof(bmap), &addr);
			
			time_i = clock();
			printf("TIMESTAMP: %.1f: Sending msg -> \"%s\" to %d\n", (float)(time_i)*1000/CLOCKS_PER_SEC, message, addr.sin_addr.s_addr);
			
		}

		SDL_Delay(1000.0/FPS);
	}
	
	deInitSDL2();
	
	return 0;

}
