#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES

#define M_PI_F (float)M_PI

#include <cmath>
#include <iostream>

#include "block.h"
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

Block* RayCast::checkFrontBack(int diffZ)
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

	return world.getBlock((int)roundf(hitX), (int)roundf(hitY), (int)roundf(hitZ));
}

Block* RayCast::checkLeftRight(int diffX)
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

	return world.getBlock((int)roundf(hitX), (int)roundf(hitY), (int)roundf(hitZ));
}

Block* RayCast::checkTopBottom(int diffY)
{
	Camera& cam = player->getCamera();
	float offset = diffY >= 0 ? -0.49f : +0.49f;

	// Normalize the player's position in the block.
	float blockY = cam.pos.y - roundf(cam.pos.y);

	// Calculate the distace to a plane perpendicular to the y-axis using the vertical rotation.
	float distance = abs(((float)diffY + offset - blockY) / tanf((-cam.rotX + 00.0f) / 180.0f * M_PI_F));

	// Resolve the hit block's coordinates.
	float hitX = cam.pos.x + distance * cosf((-cam.rotY + 90.0f) / 180.0f * M_PI_F);
	float hitY = cam.pos.y + distance * tanf((-cam.rotX + 00.0f) / 180.0f * M_PI_F);
	float hitZ = cam.pos.z + distance * sinf((-cam.rotY + 90.0f) / 180.0f * M_PI_F);

	return world.getBlock((int)roundf(hitX), (int)roundf(hitY), (int)roundf(hitZ));
}

Block* RayCast::pickBlock()
{
	Block* b = nullptr;

	int iT = 1;
	Block* bT = nullptr;
	while (iT <= 8 && (bT == nullptr || bT->isTransparent))
	{
		bT = checkTopBottom(iT);
		if (bT != nullptr && !bT->isTransparent)
		{
			if (b != nullptr && player->getEyePos().distanceSquared(bT->pos) >
				player->getEyePos().distanceSquared(b->pos))
				break;
			b = bT;
		}
		iT++;
	}

	int iF = 1;
	Block* bF = nullptr;
	while (iF <= 8 && (bF == nullptr || bF->isTransparent))
	{
		bF = checkFrontBack(iF);
		if (bF != nullptr && !bF->isTransparent)
		{
			if (b != nullptr && player->getEyePos().distanceSquared(bF->pos) >
				player->getEyePos().distanceSquared(b->pos))
				break;
			b = bF;
		}
		iF++;
	}

	int iL = 1;
	Block* bL = nullptr;
	while (iL <= 8 && (bL == nullptr || bL->isTransparent))
	{
		bL = checkLeftRight(iL);
		if (bL != nullptr && !bL->isTransparent)
		{
			if (b != nullptr && player->getEyePos().distanceSquared(bL->pos) >
				player->getEyePos().distanceSquared(b->pos))
				break;
			b = bL;
		}
		iL++;
	}

	return b;
}
