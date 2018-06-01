#include <iostream>
#include <sstream>

#include "block.h"
#include "block_stone.h"

StoneBlock::StoneBlock() : Block(
	new TexturedBlockSide(1, 0),
	new TexturedBlockSide(1, 0),
	new TexturedBlockSide(1, 0),
	new TexturedBlockSide(1, 0),
	new TexturedBlockSide(1, 0),
	new TexturedBlockSide(1, 0),
	"Stone")
{

}

CobblestoneBlock::CobblestoneBlock() : Block(
	new TexturedBlockSide(0, 1),
	new TexturedBlockSide(0, 1),
	new TexturedBlockSide(0, 1),
	new TexturedBlockSide(0, 1),
	new TexturedBlockSide(0, 1),
	new TexturedBlockSide(0, 1),
	"Cobblestone")
{

}
