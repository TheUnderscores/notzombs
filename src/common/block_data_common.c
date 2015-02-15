#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "block_data_common.h"


/* Global functions */

struct *block_common_t block_common_new(enum block_type type)
{
	struct *block_common_t newBlock = (struct block_common_t*)
		malloc(sizeof(struct block_common_t));
	newBlock->type = type;
	return newBlock;
}

void block_common_free(struct *block_common_t block)
{
	free(block);
}
