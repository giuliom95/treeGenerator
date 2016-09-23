#include "sdl.h"

SDL_Window* gWindow;
SDL_GLContext gContext;

int init_OpenGL() {
	
	GLenum error = GL_NO_ERROR;

	//Initialize Projection Matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	//glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 );
	
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR ) {
		fprintf( stderr, "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		return 0;
	}

	//Initialize Modelview Matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		fprintf( stderr, "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		return 0;
	}
	
	//Initialize clear color
	glClearColor( 0.f, 0.f, 0.f, 1.f );
	
	//Check for error
	error = glGetError();
	if( error != GL_NO_ERROR )
	{
		fprintf( stderr, "Error initializing OpenGL! %s\n", gluErrorString( error ) );
		return 0;
	}
	
	return 1;

}

int SDL_init() {

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return 0;
	} else {
		
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
		
		//Anti-aliasing
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
		
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);
		
		//Create window
		gWindow = SDL_CreateWindow( "", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL );
		if( gWindow == NULL ) {
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			return 0;
		} else {
		
			//Create context
			gContext = SDL_GL_CreateContext( gWindow );
			if( gContext == NULL ) {
				printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
				return 0;
			} else {
				//Use Vsync
				if( SDL_GL_SetSwapInterval( 1 ) < 0 ) {
					printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
				}
				
				if( FULL_SCREEN ) {
					if( SDL_SetWindowFullscreen( gWindow, SDL_WINDOW_FULLSCREEN ) < 0 ) {
						printf( "Warning: Unable to enter fullscreen mode! SDL Error: %s\n", SDL_GetError() );
					}
				}
				
				if( !MOUSE ) {
					if( SDL_ShowCursor(SDL_DISABLE) < 0 ) {
						printf( "Warning: Unable to hide cursor! SDL Error: %s\n", SDL_GetError() );
					}
				}
				
				//Initialize OpenGL
				if( !init_OpenGL() ) {
					printf( "Unable to initialize OpenGL!\n" );
					return 0;
				}
				
				//Anti-aliasing
				glHint(GL_LINE_SMOOTH_HINT, GL_NICEST );
				glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST );

				glEnable(GL_LINE_SMOOTH);
				glEnable(GL_POLYGON_SMOOTH);
			}
		}
	}

	return 1;
}


void SDL_refresh() {
	SDL_GL_SwapWindow( gWindow );
}

void SDL_clear_color_buffer() {
	glClear( GL_COLOR_BUFFER_BIT );
}


void SDL_shut_down() {
	
	SDL_DestroyWindow( gWindow );
	SDL_Quit();
	
}
