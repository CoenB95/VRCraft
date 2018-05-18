#ifndef BLOCK_GRASS_H
#define BLOCK_GRASS_H

#include<string>

#include "block.h"

class GrassBlock : public Block
{
public:
	GrassBlock();
	Block* randomTick(Block::BlockContext& adjacentBlocks) override;
};

class DirtBlock : public Block
{
public:
	DirtBlock();
};

#endif // !BLOCK_GRASS_H

