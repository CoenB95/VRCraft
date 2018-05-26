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

class SimpleGravityComponent : public GameObjectComponent
{
private:
	Chunk & world;
	bool ceiled = false;
	bool floored = false;
	float verticalAcceleration = 25.0f;
	float verticalMaxSpeed = 25.0f;
	float verticalSpeed = 0.0f;

public:
	SimpleGravityComponent(Chunk& world);
	void update(float elapsedSecond) override;
};
