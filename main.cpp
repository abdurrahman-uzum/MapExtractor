#include "main.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

const int POPULATION = 123;
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


typedef struct
{
	int x_start_index;
	int x_end_index;
	int y_start_index;
	int y_end_index;

}Gridmap;


void update_cells( unsigned char** cells, int size )
{
	//TODO: Optimise for partial updating, don't iterate the regions that are guaranteed to remain unchanged for the next clock

	static unsigned char* prev_col = (unsigned char*)malloc( sizeof( size * (**cells) ) );
	static unsigned char* curr_col = (unsigned char*)malloc( sizeof( size * (**cells) ) );
	static unsigned char* next_col = (unsigned char*)malloc( sizeof (size * (**cells) ) );

	if ( prev_col == nullptr || curr_col == nullptr || next_col == nullptr )
	{
		printf( "On function update_cells: Buffer memory allocation failed. Aborting.\n" );
		return;
	}

	memcpy( prev_col, cells[0], size+2 );
	memcpy( curr_col, cells[1], size+2 );
	memcpy( next_col, cells[2], size+2 );
	
	for ( int i = 1; i < size+1; i++ )
	{
		for ( int j = 1; j < size+1; j++ )
		{
			int neighbours	= prev_col[j-1] + curr_col[j-1] + next_col[j-1]
							+ prev_col[j]					+ next_col[j] 						
							+ prev_col[j+1] + curr_col[j+1] + next_col[j+1];

			if ( neighbours < 2 || neighbours > 3 )
			{
				cells[i][j] = 0;
			}
			else if ( neighbours == 3 )
			{
				cells[i][j] = 1;
			}
		}

		if ( i+2 >= size+2 ) //Should be a way around this...
		{
			break;
		}

		memcpy( prev_col, curr_col, size+2 );
		memcpy( curr_col, next_col, size+2 );
		memcpy( next_col, cells[i+2], size+2 );		
	}
}


int print_to_window( unsigned char** cells, int full_size, Gridmap grid, SDL_Rect unit_rect, int sub_window_width = WINDOW_WIDTH, int sub_window_height = WINDOW_HEIGHT )
{
	if ( grid.x_start_index < 0 || grid.x_end_index >= full_size || grid.y_start_index < 0 || grid.y_end_index >= full_size )
	{
		printf( "On function print_to_window:\n" );
		printf( "Invalid indecies detected, aborting printing...\n" );
		return -1;
	}
	
	SDL_SetRenderDrawColor( renderer, 0xff, 0xff, 0xff, 0xff );

	for ( int i = grid.x_start_index; i <= grid.x_end_index; i++ )
	{
		for ( int j = grid.y_start_index; j <= grid.y_end_index; j++ )
		{
			unit_rect.x = i * unit_rect.w;
			unit_rect.y = j * unit_rect.h;

			if ( cells[i+1][j+1] == 1 )
			{
				SDL_RenderFillRect( renderer, &unit_rect );
			}		
		}
	}
}


int initialise_cells( unsigned char** cells, int full_size, Gridmap grid, SDL_Rect unit_rect, SDL_Event& event )
{
	int mouse_x = 0;
	int mouse_y = 0;
	
	if ( event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN )
	{
		return 1;
	}
	else if ( event.type == SDL_MOUSEBUTTONDOWN )
	{
		SDL_GetMouseState( &mouse_x, &mouse_y );

		int i = mouse_x / unit_rect.w;
		int j = mouse_y / unit_rect.h;

		unit_rect.x = i * unit_rect.w;
		unit_rect.y = j * unit_rect.h;

		//TODO: ADD SINGLE CAPTURE IT REPEATS THE SHIT OUT OF THIS
		if ( event.button.button == SDL_BUTTON_LEFT )
		{
			cells[ 1 + grid.x_start_index + i ][ 1 + grid.y_start_index + j ] = 1;

			SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0xff, 0xff );
			SDL_RenderFillRect( renderer, &unit_rect );
		}
		else if ( event.button.button == SDL_BUTTON_RIGHT )
		{
			cells[ 1 + grid.x_start_index + i ][ 1 + grid.y_start_index + j ] = 0;
			
			SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );
			SDL_RenderFillRect( renderer, &unit_rect );
		}			
	}

	return 0;
}


void initialise_cells_random( unsigned char** cells, int full_size )
{
	if ( cells == nullptr )
	{
		printf( "On function initialise_cells_random: null pointer passed. Aborting.\n" );
		return;
	}

	for ( int i = 1; i < full_size+1; i++ )
	{
		for ( int j = 1; j < full_size+1; j++ )
		{
			cells[i][j] = rand()%2;
		}
	}
}




int main ( int argc, char** argv )
{
	if ( !init_SDL() )
	{
		return -1;
	}	

	

//--------------------------Creating and initialising cells-----------------------------//

	unsigned char** cells = NULL;

	cells = (unsigned char**)malloc( SIZE*sizeof(*cells) );
	if ( cells == NULL )
	{
		printf( "Memory allocation to cell array failed. Aborting program...\n" );
		return -1;
	}

	for ( int i = 0; i < SIZE; i++ )
	{
		cells[i] = (unsigned char*)malloc( SIZE*sizeof(**cells) );
		if ( cells[i] == NULL )
		{
			printf( "Memory allocation to column:%d is failed. Aborting program...\n", i );
			return -1;
		}
	}

	for ( int i = 0; i < SIZE; i++ )
	{
		for ( int j = 0; j < SIZE; j++ )
		{
			cells[i][j] = 0;
		}
	}

	//return 0;

//----------------------------------Auxilary Variables-----------------------------------//

	SDL_Rect unit_rect = { 0, 0, (WINDOW_WIDTH/POPULATION) , (WINDOW_HEIGHT/POPULATION) };
	Gridmap grid = { 0, POPULATION-1, 0, POPULATION-1 };
	
	bool initialised = false;	
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
				initialise_cells_random( cells, POPULATION );
				initialised = true;
			}
			else
			{
				initialised = initialise_cells( cells, POPULATION, grid, unit_rect, event );

				SDL_DrawSquareGrid( POPULATION, 0xAAAAAAFF );
				SDL_RenderPresent( renderer );
			}

			continue;			
		}

		printf( "On game loop...\n" );


		SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xff );
		SDL_RenderClear( renderer );

		print_to_window( cells, POPULATION, grid, unit_rect );
		update_cells( cells, POPULATION );
		//SDL_DrawSquareGrid( POPULATION, 0xAAAAAAFF );


		SDL_RenderPresent( renderer );
		SDL_Delay(250);
	}



	//for ( int i = 0; i < (POPULATION+2); i++ )
	//{
	//	free( cells[i] );
	//}
	//free( cells );


	quit_SDL();
	return 0;
}