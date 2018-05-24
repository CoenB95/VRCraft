#include "gameobject.h"
#include "gameobjectcomponent.h"

GameObjectComponent::GameObjectComponent()
{

}

GameObjectComponent::GameObjectComponent(GameObject* parent) : parentObject(parent)
{

}

DrawComponent::DrawComponent() : GameObjectComponent()
{

}

DrawComponent::DrawComponent(GameObject* parent) : GameObjectComponent(parent)
{

}

SpinComponent::SpinComponent(float degreesPerSec) : GameObjectComponent(),
degreesPerSec(degreesPerSec)
{

}

void SpinComponent::update(float elapsedSeconds)
{
	value += degreesPerSec * elapsedSeconds;
	while (value >= 360.0f)
		value -= 360.0f;
	parentObject->rotateY = value;
}

FollowSmoothComponent::FollowSmoothComponent(GameObject* host, float snappyness)
	: GameObjectComponent(), host(&host->position), snappyness(snappyness)
{

}

FollowSmoothComponent::FollowSmoothComponent(Vec3f* host, float snappyness)
	: GameObjectComponent(), host(host), snappyness(snappyness)
{

}

void FollowSmoothComponent::update(float elapsedSeconds)
{
	Vec3f prev = parentObject->position;
	parentObject->position = Vec3f(
		(1.0f - snappyness) * prev.x + snappyness * host->x,
		(1.0f - snappyness) * prev.y + snappyness * host->y,
		(1.0f - snappyness) * prev.z + snappyness * host->z);
}
