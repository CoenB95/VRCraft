#include <iostream>

#include "collisioncomponent.h"
#include "gameobjectcomponent.h"
#include "followcomponent.h"
#include "mob.h"

using namespace std;

Mob::Mob(Chunk& world) : world(world), collision(world), eyes(), floorDetection(world), force()
{
	addComponent(&force);
	addComponent(&floorDetection);
	addComponent(&collision);

	eyes.addComponent(FollowComponent::rotatingAndTranslating(this, 1.0f)->withOffset(Vec3f(0.0f, mobHeight, 0.0f)));
}

void Mob::jump()
{
	if (!floorDetection.isFloored())
		return;

	force.addForce(Vec3f(0.0f, 10.0f, 0.0f));
}

void Mob::move(float angleDeg, float factor, float elapsedTime)
{
	collision.move(angleDeg, factor, elapsedTime);
}

void Mob::update(float elapsedSeconds)
{
	force.addForce(Vec3f(0.0f, -0.35f, 0.0f));

	GameObject::update(elapsedSeconds);
	eyes.update(elapsedSeconds);
	
	if (floorDetection.isFloored())
		force.clearForces();
}

Steve::Steve(Chunk& world) : Mob(world)
{

}
