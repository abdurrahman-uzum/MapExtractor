#include "main.h"
#include "cells.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

const int POPULATION = 100;
const int SIZE = POPULATION+2;

const int MAX_WINDOW_WIDTH = 700;
const int MAX_WINDOW_HEIGHT = 700;

#define INIT_RANDOM true
#define SCALE_WINDOW_TO_POPULATION

#ifdef SCALE_WINDOW_TO_POPULATION
	const int WINDOW_WIDTH = POPULATION * (MAX_WINDOW_WIDTH/POPULATION);
	const int WINDOW_HEIGHT = POPULATION * (MAX_WINDOW_HEIGHT/POPULATION);
#else
	const int WINDOW_WIDTH = MAX_WINDOW_WIDTH;
	const int WINDOW_HEIGHT = MAX_WINDOW_HEIGHT;
#endif

int check_error = 0;

int main ( int argc, char** argv )
{
	if ( !init_SDL() ) { return -1; }	

	Cells cells( POPULATION );

	if ( check_error < 0 )
	{
		printf( "Process failed with code:%d\n", check_error );
		return check_error;
	}



//----------------------------------Auxilary Variables-----------------------------------//

	SDL_Rect unit_rect = { 0, 0, (WINDOW_WIDTH/POPULATION) , (WINDOW_HEIGHT/POPULATION) };
	Gridmap grid = { 0, POPULATION-1, 0, POPULATION-1 };
	
	bool initialised = false;	
	bool draw_grid = false;

	bool quit = false;
	SDL_Event event;


//------------------------------------Preperations---------------------------------------//

	srand(time(NULL));
	

	SDL_DrawSquareGrid( POPULATION, 0xAAAAAAFF );
	SDL_RenderPresent( renderer );


//------------------------------------Main Loop-------------------------------------------//

	while ( !quit )
	{
		while( SDL_PollEvent( &event ) != 0 )
		{
			if ( event.type == SDL_QUIT )
			{
				quit = true;
			}			
		}

		if ( !initialised )
		{
			if ( INIT_RANDOM )
			{
				cells.init_random();
				initialised = true;
			}
			else
			{
				initialised = cells.init( grid, unit_rect, event );

				SDL_DrawSquareGrid( POPULATION, 0xAAAAAAFF );
				SDL_RenderPresent( renderer );
			}

			continue;			
		}

		SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );
		SDL_RenderClear( renderer );

		
		cells.update();
		cells.render( grid, unit_rect );

		if ( draw_grid ) { SDL_DrawSquareGrid( POPULATION, 0xAAAAAAFF ); }


		SDL_RenderPresent( renderer );
		SDL_Delay(250);
	}


	quit_SDL();
	return 0;
}