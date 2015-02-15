#ifndef IMAGE_FUNCS_H
#define IMAGE_FUNCS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/* Loads a surface and optimizes it for a particular reference surface
 * This was, SDL won't have to format the surface every time it blits it to the
 * aforementioned reference surface
 * @param optForSurf  Surface to optimized image that will be loaded for
 * @param filepath  File path of image to be loaded and optimized
 */
SDL_Surface *loadSurface(SDL_Surface *optForSurf, char *filepath);

#endif
