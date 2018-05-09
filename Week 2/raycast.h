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

	bool checkAngleInsideRange(float angle, float min, float max);
	Block* checkFrontBack(int diffZ);
	Block* checkLeftRight(int diffX);
	Block* checkTopBottom(int diffY);
};
