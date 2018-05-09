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
	Camera& cam = player->getCamera();
	bool forwards = ((int)(-cam.rotY + 90.0f) % 360) > -90 &&
		((int)(-cam.rotY + 90.0f) % 360) < 90;
	float offset = forwards ? -0.49f : +0.49f;

	// Normalize the player's position in the block.
	float blockZ = cam.pos.z - roundf(cam.pos.z);

	// Calculate the distace to a plane perpendicular to the z-axis using the horizontal rotation.
	float distance = abs(((float)diffZ + offset - blockZ) / sinf((-cam.rotY + 90.0f) / 180.0f * M_PI));

	// Resolve the hit block's coordinates.
	float hitX = cam.pos.x + distance * cosf((-cam.rotY + 90.0f) / 180.0f * M_PI);
	float hitY = cam.pos.y + distance * tanf((-cam.rotX + 00.0f) / 180.0f * M_PI);
	float hitZ = cam.pos.z + distance * sinf((-cam.rotY + 90.0f) / 180.0f * M_PI);

	return world.getBlock((int)roundf(hitX), (int)roundf(hitY), (int)roundf(hitZ));
}

Block* RayCast::checkTopBottom(int diffY)
{
	Camera& cam = player->getCamera();
	float offset = diffY >= 0 ? -0.49f : +0.49f;

	// Normalize the player's position in the block.
	float blockY = cam.pos.y - roundf(cam.pos.y);

	// Calculate the distace to a plane perpendicular to the y-axis using the vertical rotation.
	float distance = abs(((float)diffY + offset - blockY) / tanf((-cam.rotX + 00.0f) / 180.0f * M_PI));

	// Resolve the hit block's coordinates.
	float hitX = cam.pos.x + distance * cosf((-cam.rotY + 90.0f) / 180.0f * M_PI);
	float hitY = cam.pos.y + distance * tanf((-cam.rotX + 00.0f) / 180.0f * M_PI);
	float hitZ = cam.pos.z + distance * sinf((-cam.rotY + 90.0f) / 180.0f * M_PI);

	return world.getBlock((int)roundf(hitX), (int)roundf(hitY), (int)roundf(hitZ));
}
