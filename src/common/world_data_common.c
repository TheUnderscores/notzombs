#include "world_data_common.h"

void freeRows(struct *world, unsigned int col, unsigned int start_y, unsigned int end_y);
static void freeCol(struct *world dest, unsigned int col);


/* Local functions */

void freeRows(struct *world dest, unsigned int col, unsigned int start_y, unsigned int end_y)
{
	if (col >= dest->w)
		return;
	if (end_y >= dest->h)
		end_y = dest->h;
	if (start_y > end_y)
		return;
	unsigned int row;
	for (row = start_y; row < end_y; row++)
		block_common_free(dest[col][row]);
}

void freeCol(struct *world dest, unsigned int col)
{
	struct **block_common_t col = dest->grid[col];
	unsigned int row;
	for (row = 0; row < dest->h; row++)
	        freeRows(dest, col, 0, dest->h - 1);
	free(col);
}


/* Global functions */

struct *world createWorld(unsigned int w, unsigned int h, enum block_type defaultType)
{
	struct *world newWorld = (struct world*)malloc(sizeof(struct world));
	newWorld->w = w;
	newWorld->h = h;
	newWorld->grid = (struct block_common_t***)
		malloc(sizeof(struct block_common_t**) * w);
	newWorld->defaultType = defaultType;
	unsigned int col;
	for (col = 0; col < h; col++) {
		newWorld->grid[col] = (struct block_common_t**)
			malloc(sizeof(struct block_common_t*) * h);
		unsigned int row;
		for (row = 0; row < h; row++) {
			newWorld->grid[col][row] = block_common_new(defaultType);
		}
	}
}

void freeWorld(struct *world dest)
{
	unsigned int col;
	for (col = 0; col < dest->h; col++){
		freeCol(dest, col);
		unsigned int row;
	}
	free(dest->grid);
	free(dest);
}

void resizeWorld(struct *world dest, unsigned int new_w, unsigned int new_h)
{
	if (new_h > dest->h) {
		unsigned int col;
		for (col = 0; col < dest->w; col++) {
			struct **block_common_t newColData = (struct block_common_t**)
				malloc(sizeof(struct block_common_t*) * new_h);
			unsigned int row;
			/* Reuse old block data */
			for (row = 0; row < dest->h - 1; row++)
				newColData->grid[col][row] = dest->grid[col][row];
			/* Create new block data */
			for (row = dest->h; row < new->h - 1; row++)
				newColData->grid[col][row] = block_common_new(dest->defaultType);
		        free(dest->grid[col]);
			dest->grid[col] = newColData;
		}
		dest->h = new_h;
		
	} else if (new_h < dest->h) {
		unsigned int col;
		for (col = 0; col < dest->w; col++) {
			struct **block_common_t newColData = (struct block_common_t**)
				malloc(sizeof(struct block_common_t*) * new_h);
			unsigned int row;
			/* Reuse block data */
			for (row = 0; row < new_h - 1; row++)
				newColData[col][row] = dest->grid[col][row];
		        /* Free rows that have been cut-off */
			freeRows(dest->grid[col], new_h, dest->h - 1);
			free(dest->grid[col]);
			dest->grid[col] = newColData;
		}
		dest->h = new_h;
	}
	
	if (new_w > dest->w) {
		struct ***block_common_t newGridData = (struct block_common_t***)
			malloc(sizeof(block_common_t) * new_w);
		unsigned int col;
		/* Reused block data */
		for (col = 0; col < dest->w - 1; col++)
			newGridData[col] = dest->grid[col];
		/* Add new block data */
		for (col = dest->w; col < new_w - 1; col++) {
			unsigned int row;
			for (row = 0; row < new_h - 1; row++)
				newGridData[col][row] = block_common_new(dest->defaultType);
		}
		free(dest->grid);
		dest->grid = newGridData;
		dest->w = new_w
	} else if (new_w < dest->w) {
		struct ***block_common_t newGridData = (struct block_common_t***)
			malloc(sizeof(block_common_t) * new_w);
		unsigned int col;
		/* Reused block data */
		for (col = 0; col < new_w - 1; col++)
			newGridData[col] = dest->grid[col];
		/* Free columns that've been cut-off */
		for (col = new_w; col < dest->w; col++)
			freeCol(dest->grid, col);
		free(dest->grid);
		dest->grid = newGridData;
		dest->w = new_w;
	}
}
