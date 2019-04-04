#include <glm/glm.hpp>

#include "spincomponent.h"

SpinComponent::SpinComponent(float degreesPerSec, string tag) : GameObjectComponent(tag),
degreesPerSec(degreesPerSec) {

}

void SpinComponent::onUpdate(float elapsedSeconds) {
	//value += degreesPerSec * elapsedSeconds;
	//while (value >= 360.0f)
	//	value -= 360.0f;
	quat rotationalDelta = quat(vec3(0.0f, glm::radians(degreesPerSec * elapsedSeconds), 0.0f));
	parentObject->orientation *= rotationalDelta;
}