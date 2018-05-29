#include "followcomponent.h"

FollowComponent::FollowComponent(GameObject* subject, bool translate, bool rotate, float snappyness) : GameObjectComponent(),
	subject(subject), snappyness(snappyness)
{
	this->translate = translate;
	this->rotate = rotate;
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

void FollowComponent::update(float elapsedSeconds)
{
	prev = parentObject->position;

	if (translate)
	{
		parentObject->position = Vec3f(
			snappyness * prev.x + (1.0f - snappyness) * (subject->position.x + offset.x),
			snappyness * prev.y + (1.0f - snappyness) * (subject->position.y + offset.y),
			snappyness * prev.z + (1.0f - snappyness) * (subject->position.z + offset.z));
	}

	if (rotate)
	{
		parentObject->rotateX = snappyness * parentObject->rotateX + (1.0f - snappyness) * subject->rotateX;
		parentObject->rotateY = snappyness * parentObject->rotateY + (1.0f - snappyness) * subject->rotateY;
		parentObject->rotateZ = snappyness * parentObject->rotateZ + (1.0f - snappyness) * subject->rotateZ;
	}
}

FollowComponent* FollowComponent::withOffset(Vec3f offset)
{
	this->offset = offset;
	return this;
}
