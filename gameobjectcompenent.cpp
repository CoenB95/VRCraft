#include "gameobject.h"
#include "gameobjectcomponent.h"

GameObjectComponent::GameObjectComponent()
{

}

void GameObjectComponent::setParent(GameObject* newParent) {
	parentObject = newParent;
	onAttach(parentObject);
}

SpinComponent::SpinComponent(float degreesPerSec) : GameObjectComponent(),
degreesPerSec(degreesPerSec)
{

}

void SpinComponent::onUpdate(float elapsedSeconds)
{
	value += degreesPerSec * elapsedSeconds;
	while (value >= 360.0f)
		value -= 360.0f;
	parentObject->rotateY = value;
}
