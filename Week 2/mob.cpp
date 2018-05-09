#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES

#include <cmath>
#include <iostream>

#include "mob.h"

using namespace std;

bool Mob::checkCollision(vector<Block::BlockContext> collisionBoxes, Block*(*f)(Block::BlockContext b))
{
	for (int i = 0; i < collisionBoxes.size(); i++)
	{
		Block* b = f(collisionBoxes[i]);
		if (b == nullptr || !world.isBlockTransparent(b))
			return true;
	}

	return false;
}

Mob::Mob(Chunk& world) : eyes(), world(world)
{
	int i;
	i = 0;
}

Camera& Mob::getCamera()
{
	return eyes;
}

Vec3f Mob::getEyePos()
{
	return eyes.pos;
}

// 0 degrees = North, towards positive Z
void Mob::move(float angleDeg, float factor, float elapsedTime)
{
	float chunkX = roundf(eyes.pos.x);
	float chunkZ = roundf(eyes.pos.z);
	float blockX = eyes.pos.x - chunkX;
	float blockZ = eyes.pos.z - chunkZ;
	float deltaX = (float)cos((-90 + eyes.rotY + angleDeg) / 360 * M_PI * 2) * factor;
	float deltaZ = (float)sin((90 + eyes.rotY + angleDeg) / 360 * M_PI * 2) * factor;
	blockX += deltaX;
	blockZ += deltaZ;

	Block* curFloor = world.getBlock(
		chunkX,
		roundf(eyes.pos.y - mobBlockHeight) - 1,
		chunkZ);

	if (curFloor == nullptr)
	{
		cout << "No collision-check: outside world" << endl;
	}
	else
	{
		vector<Block::BlockContext> collisionBoxes;
		Block* tempBlock = world.getAdjacentBlocks(curFloor).top;
		if (tempBlock == nullptr)
		{
			cout << "No collision-check: top level" << endl;
		}
		else
		{
			for (int h = 0; h < mobBlockHeight; h++)
			{
				if (tempBlock == nullptr)
					break;

				Block::BlockContext context = world.getAdjacentBlocks(tempBlock);
				collisionBoxes.push_back(context);
				tempBlock = context.top;
			}

			// The distance that should be kept from any walls
			GLfloat d = 0.5f - mobDiameter / 2;

			// Collision checking
			// Left
			if (checkCollision(collisionBoxes, [](Block::BlockContext b) { return b.left; }) && blockX < -d) blockX = -d;
			// Right
			if (checkCollision(collisionBoxes, [](Block::BlockContext b) { return b.right; }) && blockX > d) blockX = d;
			// Back (Block in front of our space)
			if (checkCollision(collisionBoxes, [](Block::BlockContext b) { return b.front; }) && blockZ < -d) blockZ = -d;
			// Front (Block behind our space)
			if (checkCollision(collisionBoxes, [](Block::BlockContext b) { return b.back; }) && blockZ > d) blockZ = d;
		}
	}
	eyes.pos.x = (chunkX + blockX);
	eyes.pos.z = (chunkZ + blockZ);
}

void Mob::update(float elapsedTime)
{
	Block* curFloor = world.getBlock(
		roundf(eyes.pos.x),
		roundf(eyes.pos.y - mobBlockHeight) - 1,
		roundf(eyes.pos.z));

	if (lastFloor != nullptr)
		lastFloor->mark = false;

	if (curFloor != nullptr)
		curFloor->mark = true;

	lastFloor = curFloor;

	speedY -= accelY * elapsedTime;
	eyes.pos.y += speedY * elapsedTime;

	floored = curFloor != nullptr && !world.isBlockTransparent(curFloor) &&
		eyes.pos.y - mobBlockHeight < curFloor->y + 0.5f;
	if (floored)
	{
		eyes.pos.y = (curFloor->y + 0.5f + mobBlockHeight);
		speedY = 0;
	}
}

Steve::Steve(Chunk& world) : Mob(world)
{

}
