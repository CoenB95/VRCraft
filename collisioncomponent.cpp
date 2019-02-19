#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES

#include <cmath>
#include <vector>

#include "block.h"
#include "chunk.h"
#include "collisioncomponent.h"
#include "gameobject.h"
#include "gameobjectcomponent.h"
#include "mob.h"
#include "vec.h"

BlockCollisionComponent::BlockCollisionComponent(Chunk& world) : GameObjectComponent(), world(world)
{

}

bool BlockCollisionComponent::checkCollision(vector<Block::BlockContext> collisionBoxes, Block*(*f)(Block::BlockContext b))
{
	for (GLuint i = 0; i < collisionBoxes.size(); i++)
	{
		Block* b = f(collisionBoxes[i]);
		if (b == nullptr || !world.isBlockTransparent(b))
			return true;
	}

	return false;
}

// 0 degrees = North, towards positive Z
void BlockCollisionComponent::move(float angleDeg, float factor, float elapsedTime)
{
	Mob* mob = dynamic_cast<Mob*>(parentObject);
	if (mob == nullptr)
		return;

	float chunkX = roundf(parentObject->position.x);
	float chunkZ = roundf(parentObject->position.z);
	float blockX = parentObject->position.x - chunkX;
	float blockZ = parentObject->position.z - chunkZ;
	float deltaX = (float)cos((-90 + parentObject->rotateY + angleDeg) / 360 * M_PI * 2) * factor;
	float deltaZ = (float)sin((90 + parentObject->rotateY + angleDeg) / 360 * M_PI * 2) * factor;
	blockX += deltaX;
	blockZ += deltaZ;

	Block* curFloor = world.getBlock(
		chunkX,
		roundf(parentObject->position.y) - 1,
		chunkZ);

	if (curFloor != nullptr)
	{
		vector<Block::BlockContext> collisionBoxes;
		Block* tempBlock = world.getAdjacentBlocks(curFloor).top;
		if (tempBlock != nullptr)
		{
			for (int h = 0; h < mob->mobHeight; h++)
			{
				if (tempBlock == nullptr)
					break;

				Block::BlockContext context = world.getAdjacentBlocks(tempBlock);
				collisionBoxes.push_back(context);
				tempBlock = context.top;
			}

			// The distance that should be kept from any walls
			GLfloat d = 0.5f - mob->mobDiameter / 2;

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
	parentObject->position.x = (chunkX + blockX);
	parentObject->position.z = (chunkZ + blockZ);
}

void BlockCollisionComponent::onUpdate(float elapsedSeconds)
{
	
}

FloorCollisionComponent::FloorCollisionComponent(Chunk& world) : GameObjectComponent(), world(world)
{

}

void FloorCollisionComponent::onUpdate(float elapsedTime)
{
	floored = curFloor != nullptr && !world.isBlockTransparent(curFloor) &&
		parentObject->position.y < curFloor->position.y + 0.5f;

	if (floored)
		parentObject->position.y = (curFloor->position.y + 0.5f);

	curFloor = world.getBlock(
		roundf(parentObject->position.x),
		roundf(parentObject->position.y) - 1,
		roundf(parentObject->position.z));
}

CeilingCollisionComponent::CeilingCollisionComponent(Chunk& world) : GameObjectComponent(), world(world)
{

}

void CeilingCollisionComponent::onUpdate(float elapsedTime)
{
	Mob* mob = dynamic_cast<Mob*>(parentObject);
	if (mob == nullptr)
		return;

	ceiled = curCeiling != nullptr && !world.isBlockTransparent(curCeiling) &&
		parentObject->position.y > curCeiling->position.y - 0.5f - (mob->mobDiameter / 2);
	if (ceiled)
	{
		parentObject->position.y = (curCeiling->position.y - 0.5f - (mob->mobDiameter / 2));
	}

	curCeiling = world.getBlock(
		roundf(parentObject->position.x),
		roundf(parentObject->position.y + mob->mobHeight) + 1,
		roundf(parentObject->position.z));
}
