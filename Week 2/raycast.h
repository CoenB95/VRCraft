#pragma once

#define SQDIST(a,b) (a * a + b * b)

class Chunk;
class Mob;

class PickResult
{
public:
	Block* block;
	char side;

	PickResult(Block* block, char side);
};

class RayCast
{
private:
	Mob* player;
	Chunk& world;

	bool checkAngleInsideRange(float angle, float min, float max);
	PickResult checkFrontBack(int diffZ);
	PickResult checkLeftRight(int diffX);
	PickResult checkTopBottom(int diffY);

public:
	RayCast(Mob* player, Chunk& world);

	PickResult pickBlock();
};
