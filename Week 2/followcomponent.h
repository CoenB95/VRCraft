#pragma once

#include "gameobject.h"
#include "gameobjectcomponent.h"

class FollowComponent : public GameObjectComponent
{
private:
	GameObject* subject;
	Vec3f offset;
	Vec3f prev;
	bool rotate;
	bool translate;
	float snappyness;

	FollowComponent(GameObject* subject, bool translate, bool rotate, float snappyness);

public:
	static FollowComponent* rotating(GameObject* subject, float snappyness);
	static FollowComponent* rotatingAndTranslating(GameObject* subject, float snappyness);
	static FollowComponent* translating(GameObject* subject, float snappyness);

	void update(float elapsedSeconds) override;
	FollowComponent* withOffset(Vec3f offset);
};
