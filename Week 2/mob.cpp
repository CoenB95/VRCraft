#include <iostream>

#include "collisioncomponent.h"
#include "gameobjectcomponent.h"
#include "mob.h"

using namespace std;

Mob::Mob(Chunk& world) : world(world)
{
	collision = new BlockCollisionComponent(world);
	addComponent(collision);
	addComponent(new SimpleGravityComponent(world));
}

void Mob::move(float angleDeg, float factor, float elapsedTime)
{
	collision->move(angleDeg, factor, elapsedTime);
}

void Mob::update(float elapsedSeconds)
{
	GameObject::update(elapsedSeconds);
	eyePosition = Vec3f(position.x, position.y + mobHeight, position.z);
}

Steve::Steve(Chunk& world) : Mob(world)
{

}
