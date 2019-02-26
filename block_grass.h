#ifndef BLOCK_GRASS_H
#define BLOCK_GRASS_H

#include<string>

#include "block.h"

class GrassBlock : public CubeBlock
{
public:
	GrassBlock();
	Block* randomTick(BlockContext& adjacentBlocks) override;
};

class DirtBlock : public CubeBlock
{
public:
	DirtBlock();
	Block* randomTick(BlockContext& adjacentBlocks) override;
};

#endif // !BLOCK_GRASS_H

