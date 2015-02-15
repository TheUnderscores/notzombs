#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "image_funcs.h"

SDL_Surface *loadSurface(SDL_Surface *optForSurf, char *filepath)
{
	SDL_Surface *loadedSurf = NULL;
	/* Optimized surface */
	SDL_Surface *optSurf = NULL;
	
	SDL_Surface *loadSurf = IMG_Load(filepath);
	if (loadSurf == NULL) {
		printf("Unable to load image %s! SDL_image Error:",
		       filepath,
		       IMG_GetError());
		return NULL;
	}
	
	optSurf = SDL_ConvertSurface(loadedSurf, optForSurf->format, 0);
	if (optSurf == NULL) {
		printf("Unable to optimize image %s! SDL_image Error:",
		       filepath,
		       IMG_GetError());
		return NULL;
	}
	
	SDL_FreeSurface(loadedSurf);
	
	return optSurf;
}
