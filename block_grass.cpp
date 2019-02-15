#include <iostream>
#include <sstream>

#include "block.h"
#include "block_grass.h"

GrassBlock::GrassBlock() : Block(
	new TexturedBlockSide(0, 0, 1, 1),
	new TexturedBlockSide(3, 0, 1, 1),
	new TexturedBlockSide(3, 0, 1, 1),
	new TexturedBlockSide(3, 0, 1, 1),
	new TexturedBlockSide(3, 0, 1, 1),
	new TexturedBlockSide(2, 0, 1, 1),
	"Grass")
{

}

Block* GrassBlock::randomTick(Block::BlockContext& adjacentBlocks)
{

	Block* top = adjacentBlocks.top;
	if (top != nullptr && !top->isTransparent)
	{
		DirtBlock* dirt = new DirtBlock();
		dirt->isTransparent = isTransparent;
		dirt->position = Vec3f(position.x, position.y, position.z);

		dirt->topSide->shouldRender = adjacentBlocks.top == nullptr || adjacentBlocks.top->isTransparent;
		dirt->frontSide->shouldRender = adjacentBlocks.front == nullptr || adjacentBlocks.front->isTransparent;
		dirt->rightSide->shouldRender = adjacentBlocks.right == nullptr || adjacentBlocks.right->isTransparent;
		dirt->backSide->shouldRender = adjacentBlocks.back == nullptr || adjacentBlocks.back->isTransparent;
		dirt->leftSide->shouldRender = adjacentBlocks.left == nullptr || adjacentBlocks.left->isTransparent;
		dirt->bottomSide->shouldRender = adjacentBlocks.bottom == nullptr || adjacentBlocks.bottom->isTransparent;

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
	new TexturedBlockSide(2, 0, 1, 1),
	"Dirt")
{

}
