#ifndef WORLD_DATA_COMMON_H
#define WORLD_DATA_COMMON_H

#include "block_data_common.h"

struct world {
	unsigned int w;
	unsigned int h;
	/* Array of arrays of pointers to blocks */
	struct ***block_common_t grid;
	enum block_type defaultType;
};


struct *world createWorld(unsigned int w, unsigned int h, enum block_type defaultType);
void freeWorld(struct *world dest);
void resizeWorld(struct *world dest, unsigned int new_w, unsigned int new_h);

#endif
