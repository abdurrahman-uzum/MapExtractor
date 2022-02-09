#include "main.h"
#include "cells.h"


#define SCALE_WINDOW_TO_POPULATION
#define IMAGE_PATH "gun.bmp"


SDL_Window*	  window   = NULL;
SDL_Renderer* renderer = NULL;

const int SIZE = 50;

const int MAX_WINDOW_WIDTH = 700;
const int MAX_WINDOW_HEIGHT = 700;


#ifdef SCALE_WINDOW_TO_POPULATION
	const int WINDOW_WIDTH	= SIZE * (MAX_WINDOW_WIDTH/SIZE);
	const int WINDOW_HEIGHT = SIZE * (MAX_WINDOW_HEIGHT/SIZE);
#else
	const int WINDOW_WIDTH	= MAX_WINDOW_WIDTH;
	const int WINDOW_HEIGHT = MAX_WINDOW_HEIGHT;
#endif

// Will remove this soon
int check_error = 0;





int main ( int argc, char** argv )
{
	if ( !init_SDL() ) { return -1; }	

	Cells cells( SIZE );

	if ( check_error < 0 )
	{
		fprintf( stderr, "Process failed with code:%d\n", check_error );
		return check_error;
	}


	//----------------------------------Auxiliary Variables-----------------------------------//

	SDL_Rect unit_rect = { 0, 0, (WINDOW_WIDTH/SIZE) , (WINDOW_HEIGHT/SIZE) };
	Gridmap grid = { 0, SIZE-1, 0, SIZE-1 };
	
	SDL_Event event;

	INIT_TYPE init_type = INIT_TYPE::IMAGE;

	// Control flags	
	bool quit = false;	


	//------------------------------------Preperations---------------------------------------//
	

	quit = cells.init_by_imag( IMAGE_PATH, 50, 0 );

	// Display what the initial conditions are set to
	if ( !quit )
	{
		cells.render( unit_rect );
		SDL_DrawSquareGrid( SIZE, 0xAAAAAAFF );
		SDL_RenderPresent( renderer );
	}


	while ( !quit )
	{
		while( SDL_PollEvent( &event ) != 0 )
		{
			if ( event.type == SDL_QUIT )
			{
				quit = true;
			}			

		}		
	}


	quit_SDL();
	return 0;
}