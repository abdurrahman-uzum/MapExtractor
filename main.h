#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>

#include<string.h>
#include<time.h>

#include<SDL.h>
#include<SDL_image.h>

#include<optional>

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

extern const int POPULATION;
extern const int SIZE;

extern int check_error;


enum class Color { WHITE, BLACK, RED, GREEN, BLUE };

bool init_SDL();
void quit_SDL();

void SDL_DrawSquareGrid( int cell_count, Uint32 color = 0xFFFFFFFF, int start_x = 0, int end_x = WINDOW_WIDTH, int start_y = 0, int end_y = WINDOW_HEIGHT );

typedef struct
{
	int x_start_index;
	int x_end_index;
	int y_start_index;
	int y_end_index;

}Gridmap;

enum class INIT_TYPE { USER, IMAGE, FILE, RANDOM };
