#include "followcomponent.h"

FollowComponent::FollowComponent(GameObject* subject, bool translate, bool rotate) : GameObjectComponent(), subject(subject) {
	this->translate = translate;
	this->rotate = rotate;
}

FollowComponent* FollowComponent::rotating(GameObject* subject) {
	return new FollowComponent(subject, false, true);
}

FollowComponent* FollowComponent::rotatingAndTranslating(GameObject* subject) {
	return new FollowComponent(subject, true, true);
}

FollowComponent* FollowComponent::translating(GameObject* subject) {
	return new FollowComponent(subject, true, false);
}

void FollowComponent::onUpdate(float elapsedSeconds)
{
	prev = parentObject->position;

	if (translate) {
		parentObject->position = subject->position + offset;
	}

	if (rotate) {
		parentObject->orientation = subject->orientation;
	}
}

FollowComponent* FollowComponent::withOffset(vec3 offset)
{
	this->offset = offset;
	return this;
}

SmoothComponent::SmoothComponent(bool translate, bool rotate, float snappyness) : GameObjectComponent(),
snappyness(snappyness)
{
	this->translate = translate;
	this->rotate = rotate;
}

SmoothComponent* SmoothComponent::rotating(float snappyness) {
	return new SmoothComponent(false, true, snappyness);
}

SmoothComponent* SmoothComponent::rotatingAndTranslating(float snappyness) {
	return new SmoothComponent(true, true, snappyness);
}

SmoothComponent* SmoothComponent::translating(float snappyness) {
	return new SmoothComponent(true, false, snappyness);
}

void SmoothComponent::onAttach(GameObject* newParent) {
	previousPosition = newParent->position;
	previousOrientation = newParent->orientation;
}

void SmoothComponent::onUpdate(float elapsedSeconds)
{
	if (translate) {
		parentObject->position = glm::mix(previousPosition, parentObject->position, snappyness);
	}

	if (rotate) {
		parentObject->orientation = glm::slerp(previousOrientation, parentObject->orientation, snappyness);
	}

	previousPosition = parentObject->position;
	previousOrientation = parentObject->orientation;
}
