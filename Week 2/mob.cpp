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

Camera& Mob::getEyes()
{
	return eyes;
}

// 0 degrees = North, towards positive Z
void Mob::move(float angleDeg, float factor, float elapsedTime)
{
	float chunkX = roundf(eyes.posX / world.blockSize);
	float chunkZ = roundf(eyes.posZ / world.blockSize);
	float blockX = eyes.posX - chunkX * world.blockSize;
	float blockZ = eyes.posZ - chunkZ * world.blockSize;
	float deltaX = (float)cos((-90 + eyes.rotY + angleDeg) / 360 * M_PI * 2) * factor;
	float deltaZ = (float)sin((90 + eyes.rotY + angleDeg) / 360 * M_PI * 2) * factor;
	blockX += deltaX / world.blockSize;
	blockZ += deltaZ / world.blockSize;

	Block* curFloor = world.getBlock(
		roundf(eyes.posX / world.blockSize),
		(eyes.posY - mobBlockHeight * world.blockSize) / world.blockSize,
		roundf(eyes.posZ / world.blockSize));

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

			//Block::BlockContext feetContext = world.getAdjacentBlocks(chunk.getAdjacentBlocks(curFloor).top);
			//Block::BlockContext headContext = chunk.getAdjacentBlocks(
			//	chunk.getAdjacentBlocks(chunk.getAdjacentBlocks(curFloor).top).top);

			// Collision checking
			// Left
			if (checkCollision(collisionBoxes, [](Block::BlockContext b) { return b.left; }) && blockX < -0.49f) blockX = -0.49f;
			// Right
			if (checkCollision(collisionBoxes, [](Block::BlockContext b) { return b.right; }) && blockX > 0.49f) blockX = 0.49f;
			// Back (Block in front of our space)
			if (checkCollision(collisionBoxes, [](Block::BlockContext b) { return b.front; }) && blockZ < -0.49f) blockZ = -0.49f;
			// Front (Block behind our space)
			if (checkCollision(collisionBoxes, [](Block::BlockContext b) { return b.back; }) && blockZ > 0.49f) blockZ = 0.49f;
		}
	}
	eyes.posX = (chunkX + blockX) * world.blockSize;
	eyes.posZ = (chunkZ + blockZ) * world.blockSize;
}

Steve::Steve(Chunk& world) : Mob(world)
{

}
