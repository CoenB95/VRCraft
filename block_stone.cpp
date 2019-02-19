#include <iostream>
#include <sstream>

#include "block.h"
#include "block_stone.h"

StoneBlock::StoneBlock() : CubeBlock(1, "Stone")
{

}

CobblestoneBlock::CobblestoneBlock() : CubeBlock(Block::TILES_WIDTH_COUNT + 0, "Cobblestone")
{

}
