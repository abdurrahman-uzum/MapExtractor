#pragma once

#include "main.h"

class Cells
{
	private:
		const int SIZE;
		unsigned char** cells;

		

	public:
		Cells( int population );
		~Cells();
		
		int render(SDL_Rect unit_rect );
		
		int init_by_imag( const char* path, int cell_length, int active_color );

};