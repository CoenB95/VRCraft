#pragma once

#include "chunk.h"
#include "gameobjectcomponent.h"

class BlockCollisionComponent : public GameObjectComponent
{
private:
	Chunk& world;

	bool checkCollision(vector<Block::BlockContext> collisionBoxes, Block*(*f)(Block::BlockContext b));

public:
	BlockCollisionComponent(Chunk& world);
	void move(float angleDeg, float factor, float elapsedTime);
	void update(float elapsedSecond) override;
};

class FloorCollisionComponent : public GameObjectComponent
{
private:
	Chunk& world;
	Block* curFloor;
	bool floored = false;

public:
	FloorCollisionComponent(Chunk& world);
	bool isFloored() { return floored; }
	void update(float elapsedSecond) override;
};

class CeilingCollisionComponent : public GameObjectComponent
{
private:
	Chunk& world;
	Block* curCeiling;
	bool ceiled = false;

public:
	CeilingCollisionComponent(Chunk& world);
	bool isCeiled() { return ceiled; }
	void update(float elapsedSecond) override;
};
