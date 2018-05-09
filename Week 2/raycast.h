#pragma once

#define SQDIST(a,b) (a * a + b * b)

#include "chunk.h"
#include "mob.h"

class RayCast
{
private:
	Mob* player;
	Chunk& world;

public:
	RayCast(Mob* player, Chunk& world);
	Block* checkFrontBack(int diffZ);
	Block* checkTopBottom(int diffY);
};
