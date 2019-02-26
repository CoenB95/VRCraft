#include <iostream>
#include <sstream>

#include "block.h"
#include "blocks.h"

Block* GrassBlock::randomTick(BlockContext& adjacentBlocks) {
	if (adjacentBlocks.top != nullptr && !adjacentBlocks.top->isTransparent) {
		DirtBlock* dirt = new DirtBlock();
		dirt->isTransparent = isTransparent;
		dirt->position = vec3(position.x, position.y, position.z);

		return dirt;
	}

	return nullptr;
}

Block* DirtBlock::randomTick(BlockContext& adjacentBlocks) {
	//Check no block directly above.
	if (adjacentBlocks.top == nullptr || adjacentBlocks.top->isTransparent) {
		//Check adjacent to grass.
		if (false) {
			DirtBlock* dirt = new DirtBlock();
			dirt->isTransparent = isTransparent;
			dirt->position = vec3(position.x, position.y, position.z);

			return dirt;
		}
	}

	return nullptr;
}
