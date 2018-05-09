#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES

#include <cmath>
#include <iostream>

#include "block.h"
#include "raycast.h"

RayCast::RayCast(Mob* player, Chunk& world) : player(player), world(world)
{

}

Block* RayCast::checkFrontBack(int diffZ)
{
	bool forwards = ((int)(-player->getEyes().rotY + 90.0f) % 360) > -90 &&
		((int)(-player->getEyes().rotY + 90.0f) % 360) < 90;
	float offset = forwards ? -0.49f : +0.49f;

	// Normalize the player's position in the block.
	float blockZ = player->getEyes().posZ - roundf(player->getEyes().posZ);

	// Calculate the distace to a plane perpendicular to the z-axis using the horizontal rotation.
	float distance = abs(((float)diffZ + offset - blockZ) / sinf((-player->getEyes().rotY + 90.0f) / 180.0f * M_PI));

	// Resolve the hit block's coordinates.
	float hitX = player->getEyes().posX + distance * cosf((-player->getEyes().rotY + 90.0f) / 180.0f * M_PI);
	float hitY = player->getEyes().posY + distance * tanf((-player->getEyes().rotX + 00.0f) / 180.0f * M_PI);
	float hitZ = player->getEyes().posZ + distance * sinf((-player->getEyes().rotY + 90.0f) / 180.0f * M_PI);

	return world.getBlock((int)roundf(hitX), (int)roundf(hitY), (int)roundf(hitZ));
}

Block* RayCast::checkTopBottom(int diffY)
{
	float offset = diffY >= 0 ? -0.49f : +0.49f;

	// Normalize the player's position in the block.
	float blockY = player->getEyes().posY - roundf(player->getEyes().posY);

	// Calculate the distace to a plane perpendicular to the y-axis using the vertical rotation.
	float distance = abs(((float)diffY + offset - blockY) / tanf((-player->getEyes().rotX + 00.0f) / 180.0f * M_PI));

	// Resolve the hit block's coordinates.
	float hitX = player->getEyes().posX + distance * cosf((-player->getEyes().rotY + 90.0f) / 180.0f * M_PI);
	float hitY = player->getEyes().posY + distance * tanf((-player->getEyes().rotX + 00.0f) / 180.0f * M_PI);
	float hitZ = player->getEyes().posZ + distance * sinf((-player->getEyes().rotY + 90.0f) / 180.0f * M_PI);

	return world.getBlock((int)roundf(hitX), (int)roundf(hitY), (int)roundf(hitZ));
}
