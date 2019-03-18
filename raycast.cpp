#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES

#define M_PI_F (float)M_PI

#include <cmath>
#include <iostream>

#include "block.h"
#include "chunk.h"
#include "mob.h"
#include "raycast.h"

RayCast::RayCast(Mob* player, Chunk& world) : player(player), world(world)
{

}

bool RayCast::checkAngleInsideRange(float angle, float min, float max)
{
	while (angle >= 360) angle -= 360;
	while (angle < 0) angle += 360;
	if (max > min)
	{
		return angle >= min && angle < max;
	}
	else
	{
		return (angle >= min && angle < 360) || (angle >= 0 && angle < max);
	}
}

PickResult RayCast::checkFrontBack(int diffZ)
{
	/*Vec3f cam = player->eyes.position;
	bool forwards = checkAngleInsideRange(player->rotateY, 270, 90);

	// Normalize the player's position in the block.
	float blockZ = cam.z - roundf(cam.z);
	if (!forwards) blockZ *= -1;

	// Calculate the distace to a plane perpendicular to the z-axis using the horizontal rotation.
	float distance = abs(((float)diffZ - 0.499f - blockZ) / sinf((-player->rotateY + 90.0f) / 180.0f * M_PI_F));

	// Resolve the hit block's coordinates.
	float hitX = cam.x + distance * cosf((-player->rotateY + 90.0f) / 180.0f * M_PI_F);
	float hitY = cam.y + distance * tanf((-player->rotateX + 00.0f) / 180.0f * M_PI_F);
	float hitZ = cam.z + distance * sinf((-player->rotateY + 90.0f) / 180.0f * M_PI_F);

	return PickResult(world.getBlock((int)roundf(hitX), (int)roundf(hitY), (int)roundf(hitZ)),
		forwards ? Block::BlockContext::FRONT_SIDE : Block::BlockContext::BACK_SIDE);*/

	return PickResult(nullptr, -1);
}

PickResult RayCast::checkLeftRight(int diffX)
{
	/*Vec3f cam = player->eyes.position;
	bool right = checkAngleInsideRange(player->rotateY, 0, 180);

	// Normalize the player's position in the block.
	float blockX = cam.x - roundf(cam.x);
	if (!right) blockX *= -1;

	// Calculate the distace to a plane perpendicular to the x-axis using the horizontal rotation.
	float distance = abs(((float)diffX - 0.499f - blockX) / cosf((-player->rotateY + 90.0f) / 180.0f * M_PI_F));

	// Resolve the hit block's coordinates.
	float hitX = cam.x + distance * cosf((-player->rotateY + 90.0f) / 180.0f * M_PI_F);
	float hitY = cam.y + distance * tanf((-player->rotateX + 00.0f) / 180.0f * M_PI_F);
	float hitZ = cam.z + distance * sinf((-player->rotateY + 90.0f) / 180.0f * M_PI_F);

	return PickResult(world.getBlock((int)roundf(hitX), (int)roundf(hitY), (int)roundf(hitZ)),
		right ? Block::BlockContext::LEFT_SIDE : Block::BlockContext::RIGHT_SIDE);*/

	return PickResult(nullptr, -1);
}

PickResult RayCast::checkTopBottom(int diffY)
{
	/*Vec3f cam = player->eyes.position;
	bool downwards = checkAngleInsideRange(player->rotateX, 180, 0);

	// Normalize the player's position in the block.
	float blockY = cam.y - roundf(cam.y);
	if (!downwards) blockY *= -1;

	// Calculate the distace to a plane perpendicular to the y-axis using the vertical rotation.
	float distance = abs(((float)diffY -0.499f - blockY) / tanf((-player->rotateX + 00.0f) / 180.0f * M_PI_F));

	// Resolve the hit block's coordinates.
	float hitX = cam.x + distance * cosf((-player->rotateY + 90.0f) / 180.0f * M_PI_F);
	float hitY = cam.y + distance * tanf((-player->rotateX + 00.0f) / 180.0f * M_PI_F);
	float hitZ = cam.z + distance * sinf((-player->rotateY + 90.0f) / 180.0f * M_PI_F);

	return PickResult(world.getBlock((int)roundf(hitX), (int)roundf(hitY), (int)roundf(hitZ)),
		downwards ? Block::BlockContext::BOTTOM_SIDE : Block::BlockContext::TOP_SIDE);*/

	return PickResult(nullptr, -1);
}

PickResult RayCast::pickBlock()
{
	PickResult b(nullptr, -1);

	/*int iT = 1;
	PickResult bT(nullptr, -1);
	while (iT <= 8 && (bT.block == nullptr || bT.block->isTransparent))
	{
		bT = checkTopBottom(iT);
		if (bT.block != nullptr && !bT.block->isTransparent)
		{
			if (b.block != nullptr && player->eyes.position.distanceSquared(bT.block->position) >
				player->eyes.position.distanceSquared(b.block->position))
				break;
			b = bT;
		}
		iT++;
	}

	int iF = 1;
	PickResult bF(nullptr, -1);
	while (iF <= 8 && (bF.block == nullptr || bF.block->isTransparent))
	{
		bF = checkFrontBack(iF);
		if (bF.block != nullptr && !bF.block->isTransparent)
		{
			if (b.block != nullptr && player->eyes.position.distanceSquared(bF.block->position) >
				player->eyes.position.distanceSquared(b.block->position))
				break;
			b = bF;
		}
		iF++;
	}

	int iL = 1;
	PickResult bL(nullptr, -1);
	while (iL <= 8 && (bL.block == nullptr || bL.block->isTransparent))
	{
		bL = checkLeftRight(iL);
		if (bL.block != nullptr && !bL.block->isTransparent)
		{
			if (b.block != nullptr && player->eyes.position.distanceSquared(bL.block->position) >
				player->eyes.position.distanceSquared(b.block->position))
				break;
			b = bL;
		}
		iL++;
	}*/

	/*cout << "Hit side: ";
	switch (b.side)
	{
	case Block::BlockContext::TOP_SIDE:
		cout << "Top"; break;
	case Block::BlockContext::FRONT_SIDE:
		cout << "Front"; break;
	case Block::BlockContext::RIGHT_SIDE:
		cout << "Right"; break;
	case Block::BlockContext::BOTTOM_SIDE:
		cout << "Bottom"; break;
	case Block::BlockContext::BACK_SIDE:
		cout << "Back"; break;
	case Block::BlockContext::LEFT_SIDE:
		cout << "Left"; break;
	default:
		cout << "Unknown"; break;
	}
	cout << endl;*/

	return b;
}

PickResult::PickResult(Block* block, char side) : block(block), side(side)
{

}
