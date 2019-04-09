#include <iostream>

#include "gameobjectcomponent.h"
#include "followcomponent.h"
#include "gameobject.h"
#include "mob.h"
#include "stack.h"
#include "world.h"

using namespace std;

Mob::Mob(World* world) : world(world) {
	//eyes.addComponent(FollowComponent::rotatingAndTranslating(this)->withOffset(vec3(0.0f, mobHeight, 0.0f)));
}

/*void Mob::jump()
{
	if (!floorDetection.isFloored())
		return;

	force.addForce(vec3(0.0f, 10.0f, 0.0f));
}

void Mob::move(float angleDeg, float factor, float elapsedTime)
{
	//collision.move(angleDeg, factor, elapsedTime);
}

void Mob::update(float elapsedSeconds)
{
	//force.addForce(vec3(0.0f, -0.35f, 0.0f));

	GameObject::update(elapsedSeconds);
	eyes.update(elapsedSeconds);
	
	if (floorDetection.isFloored())
		force.clearForces();
}*/

Steve::Steve(World* world) : Mob(world) {

}
