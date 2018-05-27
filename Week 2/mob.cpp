#include <iostream>

#include "collisioncomponent.h"
#include "gameobjectcomponent.h"
#include "mob.h"

using namespace std;

Mob::Mob(Chunk& world) : world(world), collision(world), floorDetection(world), force()
{
	addComponent(&force);
	addComponent(&floorDetection);
	addComponent(&collision);
}

void Mob::jump()
{
	if (!floorDetection.isFloored())
		return;

	force.addForce(Vec3f(0.0f, 7.5f, 0.0f));
}

void Mob::move(float angleDeg, float factor, float elapsedTime)
{
	collision.move(angleDeg, factor, elapsedTime);
}

void Mob::update(float elapsedSeconds)
{
	force.addForce(Vec3f(0.0f, -0.35f, 0.0f));
	GameObject::update(elapsedSeconds);
	eyePosition = Vec3f(position.x, position.y + mobHeight, position.z);
	if (floorDetection.isFloored())
		force.clearForces();
}

Steve::Steve(Chunk& world) : Mob(world)
{

}
