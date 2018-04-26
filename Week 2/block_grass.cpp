#include <iostream>
#include <sstream>

#include "block.h"
#include "block_grass.h"

GrassBlock::GrassBlock() : Block(
	new TexturedBlockSide(0, 0, 1, 1),
	new TexturedBlockSide(5, 0, 1, 1),
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
		dirt->isTransparent = isTransparent;
		dirt->setPosition(x, y, z);
		return dirt;
	}

	return nullptr;
}

string GrassBlock::toString() const
{
	stringstream ss;
	ss << "Grass" << "{" << getPositionString() << "}";
	return ss.str();
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
