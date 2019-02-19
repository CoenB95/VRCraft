
#include "gameobject.h"
#include "gameobjectcomponent.h"
#include "forcecomponent.h"

ForceComponent::ForceComponent() : GameObjectComponent()
{

}

void ForceComponent::addForce(vec3 value)
{
	newForce += value;
}

void ForceComponent::clearForces()
{
	curForce = vec3();
}

void ForceComponent::onUpdate(float elapsedSeconds)
{
	curForce += newForce;
	parentObject->position += curForce * elapsedSeconds;

	newForce = vec3();
}

SimpleGravityComponent::SimpleGravityComponent() : ForceComponent()
{

}

void SimpleGravityComponent::onUpdate(float elapsedSeconds)
{
	addForce(vec3(0.0f, -0.35f, 0.0f));
	ForceComponent::onUpdate(elapsedSeconds);
}
