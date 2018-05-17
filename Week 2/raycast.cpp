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
	Camera& cam = player->getCamera();
	bool forwards = checkAngleInsideRange(cam.rotY, 270, 90);

	// Normalize the player's position in the block.
	float blockZ = cam.pos.z - roundf(cam.pos.z);
	if (!forwards) blockZ *= -1;

	// Calculate the distace to a plane perpendicular to the z-axis using the horizontal rotation.
	float distance = abs(((float)diffZ - 0.49f - blockZ) / sinf((-cam.rotY + 90.0f) / 180.0f * M_PI_F));

	// Resolve the hit block's coordinates.
	float hitX = cam.pos.x + distance * cosf((-cam.rotY + 90.0f) / 180.0f * M_PI_F);
	float hitY = cam.pos.y + distance * tanf((-cam.rotX + 00.0f) / 180.0f * M_PI_F);
	float hitZ = cam.pos.z + distance * sinf((-cam.rotY + 90.0f) / 180.0f * M_PI_F);

	return PickResult(world.getBlock((int)roundf(hitX), (int)roundf(hitY), (int)roundf(hitZ)),
		forwards ? PickResult::FRONT_SIDE : PickResult::BACK_SIDE);
}

PickResult RayCast::checkLeftRight(int diffX)
{
	Camera& cam = player->getCamera();
	bool right = checkAngleInsideRange(cam.rotY, 0, 180);

	// Normalize the player's position in the block.
	float blockX = cam.pos.x - roundf(cam.pos.x);
	if (!right) blockX *= -1;

	// Calculate the distace to a plane perpendicular to the x-axis using the horizontal rotation.
	float distance = abs(((float)diffX - 0.49f - blockX) / cosf((-cam.rotY + 90.0f) / 180.0f * M_PI_F));

	// Resolve the hit block's coordinates.
	float hitX = cam.pos.x + distance * cosf((-cam.rotY + 90.0f) / 180.0f * M_PI_F);
	float hitY = cam.pos.y + distance * tanf((-cam.rotX + 00.0f) / 180.0f * M_PI_F);
	float hitZ = cam.pos.z + distance * sinf((-cam.rotY + 90.0f) / 180.0f * M_PI_F);

	return PickResult(world.getBlock((int)roundf(hitX), (int)roundf(hitY), (int)roundf(hitZ)),
		right ? PickResult::LEFT_SIDE : PickResult::RIGHT_SIDE);
}

PickResult RayCast::checkTopBottom(int diffY)
{
	Camera& cam = player->getCamera();
	bool upwards = checkAngleInsideRange(cam.rotX, 0, 180);
	float offset = diffY >= 0 ? -0.49f : +0.49f;

	// Normalize the player's position in the block.
	float blockY = cam.pos.y - roundf(cam.pos.y);

	// Calculate the distace to a plane perpendicular to the y-axis using the vertical rotation.
	float distance = abs(((float)diffY + offset - blockY) / tanf((-cam.rotX + 00.0f) / 180.0f * M_PI_F));

	// Resolve the hit block's coordinates.
	float hitX = cam.pos.x + distance * cosf((-cam.rotY + 90.0f) / 180.0f * M_PI_F);
	float hitY = cam.pos.y + distance * tanf((-cam.rotX + 00.0f) / 180.0f * M_PI_F);
	float hitZ = cam.pos.z + distance * sinf((-cam.rotY + 90.0f) / 180.0f * M_PI_F);

	return PickResult(world.getBlock((int)roundf(hitX), (int)roundf(hitY), (int)roundf(hitZ)),
		upwards ? PickResult::TOP_SIDE : PickResult::BOTTOM_SIDE);
}

PickResult RayCast::pickBlock()
{
	PickResult b(nullptr, -1);

	int iT = 1;
	PickResult bT(nullptr, -1);
	while (iT <= 8 && (bT.block == nullptr || bT.block->isTransparent))
	{
		bT = checkTopBottom(iT);
		if (bT.block != nullptr && !bT.block->isTransparent)
		{
			if (b.block != nullptr && player->getEyePos().distanceSquared(bT.block->pos) >
				player->getEyePos().distanceSquared(b.block->pos))
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
			if (b.block != nullptr && player->getEyePos().distanceSquared(bF.block->pos) >
				player->getEyePos().distanceSquared(b.block->pos))
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
			if (b.block != nullptr && player->getEyePos().distanceSquared(bL.block->pos) >
				player->getEyePos().distanceSquared(b.block->pos))
				break;
			b = bL;
		}
		iL++;
	}

	cout << "Hit side: ";
	switch (b.side)
	{
	case PickResult::TOP_SIDE:
		cout << "Top"; break;
	case PickResult::FRONT_SIDE:
		cout << "Front"; break;
	case PickResult::RIGHT_SIDE:
		cout << "Right"; break;
	case PickResult::BOTTOM_SIDE:
		cout << "Bottom"; break;
	case PickResult::BACK_SIDE:
		cout << "Back"; break;
	case PickResult::LEFT_SIDE:
		cout << "Left"; break;
	default:
		cout << "Unknown"; break;
	}
	cout << endl;

	return b;
}

PickResult::PickResult(Block* block, char side) : block(block), side(side)
{

}
