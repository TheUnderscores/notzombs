#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "block_data.h"

/* Array of block properties.  */
static struct block_props *type_data;
static const int TEXTURE_W = 32;
static const int TEXTURE_H = 32;

/* Local functions */



/* Global functions */

void block_data_init(void)
{
	/* TODO: Need images for block textures */
	
        type_data = malloc(sizeof(struct block_props) * BLOCK_TYPE_TOTAL);
        
	type_data[BLOCK_TYPE_WALL] = (struct block_props) {
		.texture = loadSurface(""),
		.do_collision = 1
	};
	
	type_data[BLOCK_TYPE_FLOOR] = (struct block_props) {
		.texture = loadSurface(""),
		.do_collision = 0
	};
	
	/* Adjust resolution of all images */
	SDL_Rect textureSize = {
		.x = 0,
		.y = 0,
	        .w = TEXTURE_W,
		.h = TEXTURE_H
	};
}

struct block_props get_block_props(enum block_type type)
{
	return type_data[type];
}

void block_data_free(void)
{
	int blockNum;
	for  (blockNum = 0; blockNum < BLOCK_TYPE_TOTAL; blockNum++) {
		SDL_FreeSurface(type_data[blockNum].texture);
	}
	free(type_data);
}
