#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>

#define WINDOW_WIDTH  	128.0
#define WINDOW_HEIGHT 	64.0
#define FPS			  	60.0

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Event ev;

extern SDL_Texture* oledTarget;

extern bool running;

extern float dz;
extern float angle;

extern uint32_t pixels[128 * 64];
extern uint8_t bmap[1024];  // 128 * 8(pages)

struct point{
	float x, y, z;
	float ox, oy, oz; 
	float s;
	float px, py, pz;
	SDL_Rect r;
};

//Function prototypes

void initSDL2();

void deInitSDL2();

struct point createPoint(float x, float y, float z, float size);

void updatePoint(struct point *p);

struct point *rotatePoint(struct point *p, float angle);

