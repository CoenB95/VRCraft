#include <iostream>
#include <sstream>

#include "block.h"
#include "block_grass.h"

GrassBlock::GrassBlock() : CubeBlock(0, 3, 3, 3, 3, 2, "Grass")
{

}

Block* GrassBlock::randomTick(Block::BlockContext& adjacentBlocks)
{

	Block* top = adjacentBlocks.top;
	if (top != nullptr && !top->isTransparent)
	{
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
