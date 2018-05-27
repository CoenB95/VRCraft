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
