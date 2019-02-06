
#include "gameobject.h"
#include "gameobjectcomponent.h"
#include "forcecomponent.h"
#include "vec.h"

ForceComponent::ForceComponent() : GameObjectComponent()
{

}

void ForceComponent::addForce(Vec3f value)
{
	newForce += value;
}

void ForceComponent::clearForces()
{
	curForce = Vec3f();
}

void ForceComponent::update(float elapsedSeconds)
{
	curForce += newForce;
	parentObject->position += curForce * elapsedSeconds;

	newForce = Vec3f();
}

SimpleGravityComponent::SimpleGravityComponent() : ForceComponent()
{

}

void SimpleGravityComponent::update(float elapsedSeconds)
{
	addForce(Vec3f(0.0f, -0.35f, 0.0f));
	ForceComponent::update(elapsedSeconds);
}
