#pragma once

#include "gameobjectcomponent.h"
#include "block.h"

#include <vector>
using namespace std;

class World;

class BlockCollisionComponent : public GameObjectComponent
{
private:
	World& world;

	bool checkCollision(vector<BlockContext> collisionBoxes, Block*(*f)(BlockContext b));

public:
	BlockCollisionComponent(World& world);
	void move(float angleDeg, float factor, float elapsedTime);
	void onUpdate(float elapsedSecond) override;
};

class FloorCollisionComponent : public GameObjectComponent
{
private:
	World& world;
	Block* curFloor;
	bool floored = false;

public:
	FloorCollisionComponent(World& world);
	bool isFloored() { return floored; }
	void onUpdate(float elapsedSecond) override;
};

class CeilingCollisionComponent : public GameObjectComponent
{
private:
	World& world;
	Block* curCeiling;
	bool ceiled = false;

public:
	CeilingCollisionComponent(World& world);
	bool isCeiled() { return ceiled; }
	void onUpdate(float elapsedSecond) override;
};
