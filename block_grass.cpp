#include <iostream>
#include <sstream>

#include "block.h"
#include "block_grass.h"

GrassBlock::GrassBlock() : CubeBlock(0, 3, 3, 3, 3, 2, "Grass")
{

}

Block* GrassBlock::randomTick(BlockContext& adjacentBlocks) {
	if (adjacentBlocks.top != nullptr && !adjacentBlocks.top->isTransparent) {
		DirtBlock* dirt = new DirtBlock();
		dirt->isTransparent = isTransparent;
		dirt->position = vec3(position.x, position.y, position.z);

		return dirt;
	}

	return nullptr;
}

DirtBlock::DirtBlock() : CubeBlock(2, 2, 2, 2, 2, 2, "Dirt")
{

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
