#ifndef BLOCK_DATA_COMMON_H
#define BLOCK_DATA_COMMON_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/* Used as array indexes. Don't change the numbers */
enum block_type {
	BLOCK_TYPE_WALL = 0,
	BLOCK_TYPE_FLOOR,
	
	/* This is the total number of block types */
	BLOCK_TYPE_TOTAL
};

struct block_common_t {
	enum block_type type;
};


struct block_common_t* block_common_new(enum block_type type);
void block_common_free(struct block_common_t *block);

#endif
