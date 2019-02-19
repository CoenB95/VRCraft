#include "followcomponent.h"

FollowComponent::FollowComponent(GameObject* subject, bool translate, bool rotate, float snappyness) : GameObjectComponent(),
	subject(subject)
{
	this->translate = translate;
	this->rotate = rotate;
	this->snappyness = snappyness;
}

FollowComponent* FollowComponent::rotating(GameObject* subject, float snappyness)
{
	return new FollowComponent(subject, false, true, snappyness);
}

FollowComponent* FollowComponent::rotatingAndTranslating(GameObject* subject, float snappyness)
{
	return new FollowComponent(subject, true, true, snappyness);
}

FollowComponent* FollowComponent::translating(GameObject* subject, float snappyness)
{
	return new FollowComponent(subject, true, false, snappyness);
}

void FollowComponent::onUpdate(float elapsedSeconds)
{
	prev = parentObject->position;

	if (translate)
	{
		parentObject->position = vec3(
			(1.0f - snappyness) * prev.x + snappyness * (subject->position.x + offset.x),
			(1.0f - snappyness) * prev.y + snappyness * (subject->position.y + offset.y),
			(1.0f - snappyness) * prev.z + snappyness * (subject->position.z + offset.z));
	}

	if (rotate)
	{
		parentObject->rotateX = (1.0f - snappyness) * parentObject->rotateX + snappyness * subject->rotateX;
		parentObject->rotateY = (1.0f - snappyness) * parentObject->rotateY + snappyness * subject->rotateY;
		parentObject->rotateZ = (1.0f - snappyness) * parentObject->rotateZ + snappyness * subject->rotateZ;
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

SmoothComponent* SmoothComponent::rotating(float snappyness)
{
	return new SmoothComponent(false, true, snappyness);
}

SmoothComponent* SmoothComponent::rotatingAndTranslating(float snappyness)
{
	return new SmoothComponent(true, true, snappyness);
}

SmoothComponent* SmoothComponent::translating(float snappyness)
{
	return new SmoothComponent(true, false, snappyness);
}

void SmoothComponent::onUpdate(float elapsedSeconds)
{
	if (translate)
	{
		parentObject->position = vec3(
			(1.0f - snappyness) * prev.x + snappyness * (parentObject->position.x),
			(1.0f - snappyness) * prev.y + snappyness * (parentObject->position.y),
			(1.0f - snappyness) * prev.z + snappyness * (parentObject->position.z));
	}

	if (rotate)
	{
		parentObject->rotateX = (1.0f - snappyness) * prevRotX + snappyness * parentObject->rotateX;
		parentObject->rotateY = (1.0f - snappyness) * prevRotY + snappyness * parentObject->rotateY;
		parentObject->rotateZ = (1.0f - snappyness) * prevRotZ + snappyness * parentObject->rotateZ;
	}

	prev = parentObject->position;
	prevRotX = parentObject->rotateX;
	prevRotY = parentObject->rotateY;
	prevRotZ = parentObject->rotateZ;
}
