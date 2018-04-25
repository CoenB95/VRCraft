#include <iostream>

#include "block.h"
#include "block_grass.h"

GrassBlock::GrassBlock() : Block(
	new TexturedBlockSide(0, 0, 1, 1),
	new TexturedBlockSide(3, 0, 1, 1),
	new TexturedBlockSide(3, 0, 1, 1),
	new TexturedBlockSide(3, 0, 1, 1),
	new TexturedBlockSide(3, 0, 1, 1),
	new TexturedBlockSide(2, 0, 1, 1))
{

}

Block* GrassBlock::randomTick(Block::BlockContext& adjacentBlocks)
{

	Block* top = adjacentBlocks.top;
	if (top != nullptr && !top->isTransparent)
	{
		DirtBlock* dirt = new DirtBlock();
		dirt->setPosition(x, y, z);
		return dirt;
	}

	return nullptr;
}

DirtBlock::DirtBlock() : Block(
	new TexturedBlockSide(2, 0, 1, 1),
	new TexturedBlockSide(2, 0, 1, 1),
	new TexturedBlockSide(2, 0, 1, 1),
	new TexturedBlockSide(2, 0, 1, 1),
	new TexturedBlockSide(2, 0, 1, 1),
	new TexturedBlockSide(2, 0, 1, 1))
{

}
