#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdint.h>
#include "renderer.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event ev;
SDL_Texture* oledTarget = NULL;

bool running = true;

float dz = 1.0f;
float angle = 0.0f;

uint32_t pixels[128 * 64];
uint8_t bmap[1024];

void initSDL2(){
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
		printf("SDL2 init failed! \n");
	}
	if(!(window = SDL_CreateWindow("3D Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_BORDERLESS))){
		printf("Window creation failed!\n");
	}
	
	if(!(renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED))){
		printf("Renderer creation failed! \n");
	}
	
	if(!(oledTarget = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, WINDOW_WIDTH, WINDOW_HEIGHT))){
		printf("Failed to create OLED Target!\n");
	}
}

void deInitSDL2(){
	// for(int i = 0; i < (int)(sizeof(bmap)); i++){
	// 	printf("0x%02X, ", bmap[i]);
	// 	// printf("0x%08X\n", pixels[i]);

	// }
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

struct point createPoint(float x, float y, float z, float size){
	struct point p = {
		.x = x, .y = y, .z = z,
		.ox = x, .oy = y, .oz = z, 
		.pz = z, .s = size
	};

	p.r = (SDL_Rect) { WINDOW_WIDTH*(p.x+1)/2 - p.s/2, WINDOW_HEIGHT*(1 - (p.y+1)/2) - p.s/2, p.s, p.s };
	SDL_RenderFillRect(renderer, &p.r);
	
	return p;
}

void updatePoint(struct point *p){
	p->pz = p->z + dz;
	p->px = p->x/p->pz;
	p->py = p->y/p->pz;
	p->r = (SDL_Rect) { WINDOW_WIDTH*(p->px+1)/2 - p->s/2, WINDOW_HEIGHT* (1 - (p->py+1)/2) - p->s/2, p->s, p->s };
}

struct point *rotatePoint(struct point *p, float angle){
    float x = p->ox;
    float z = p->oz-2.5;  
    
    p->x = x * cos(angle) - z * sin(angle);
    p->z = x * sin(angle) + z * cos(angle);
    
    return p;
}