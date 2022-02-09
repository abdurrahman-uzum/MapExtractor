#include "cells.h"

Cells::Cells( int population ) : SIZE(population)
{
	//TODO: Add proper memory allocation failure handling.

	// Allocate main array
	cells = (unsigned char**)malloc( SIZE*sizeof(*cells) );
	if ( cells == NULL )
	{
		fprintf( stderr, "ERROR On function Cells::Cells Failed to allocate memory on cells\n" );
		check_error = -1;
	}
	else
	{
		// Allocate columns
		for ( int i = 0; i < SIZE; i++ )
		{
			cells[i] = (unsigned char*)malloc( SIZE*sizeof(**cells) );
			if ( cells[i] == NULL )
			{
				fprintf( stderr, "ERROR On function Cells::Cells Failed to allocate memory on columns\n" );
				check_error = -2;
			}
		}

		if ( check_error >= 0 )
		{
			// Initialise all cells to be 0, isn't necessary, just a redundant error checking
			for ( int i = 0; i < SIZE; i++ )
			{
				for ( int j = 0; j < SIZE; j++ )
				{
					cells[i][j] = 0;
				}
			}
		}
	}	
}


Cells::~Cells()
{
	for ( int i = 0; i < SIZE; i++ )
	{
		free( cells[i] );
		cells[i] = NULL;
	}
	free( cells );
	cells = NULL;
}


int Cells::render( SDL_Rect unit_rect )
{	
	SDL_SetRenderDrawColor( renderer, 0xff, 0xff, 0xff, 0xff );

	for ( int i = 0; i < SIZE; i++ )
	{
		for ( int j = 0; j < SIZE; j++ )
		{
			unit_rect.x = i * unit_rect.w;
			unit_rect.y = j * unit_rect.h;

			if ( cells[i][j] == 1 )
			{
				SDL_RenderFillRect( renderer, &unit_rect );
			}		
		}
	}

	return 0;
}


int Cells::init_by_imag( const char* path, int cell_length, int active_color )
{
	int offset = cell_length / 2;

	SDL_Surface* image_surface = SDL_LoadBMP( path );
	if ( image_surface == NULL )
	{
		fprintf( stderr, "ERROR: On function Cells::init_by_imag Failed to load image: %s\n", SDL_GetError() );
		return -1;
	}
	else if ( image_surface->format->BytesPerPixel != 1 )
	{
		fprintf( stderr, "ERROR: On function Cells::init_by_imag Only accepting monochrome bmp images. bkz->Paint, her þeyi devletten bekleme...\n" );
		return -1;
	}
	else if ( image_surface->h / cell_length >= SIZE  ||  image_surface->w / cell_length >= SIZE )
	{
		fprintf( stderr, "WARNING: On function Cells::init_by_imag <%s> contains too much information and will be truncated.\n", path );
	}

	// Iterate through selected pixels and write to the cells. Exits if the end of image or cells array is encountered.
	for ( int j = 0; j < SIZE; j++ )
	{
		int grab_y = offset + j*cell_length;
		if ( grab_y >= image_surface->h ) { break; }


		for ( int i = 0; i < SIZE; i++ )
		{
			int grab_x = offset + i*cell_length;
			if ( grab_x >= image_surface->w ) { break; }

			Uint8* index = (Uint8*)image_surface->pixels + grab_y*image_surface->pitch + grab_x;
			cells[i][j] = ( *index == active_color ); 
		}
	}

	// Turns out you don't free( an SDL surface pointer ) ...
	SDL_FreeSurface( image_surface );
	image_surface = NULL;

	return 0;
}






