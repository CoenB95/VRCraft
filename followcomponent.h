#pragma once

#include "gameobject.h"
#include "gameobjectcomponent.h"

class FollowComponent : public GameObjectComponent
{
private:
	GameObject* subject;
	vec3 offset;
	vec3 prev;
	bool rotate;
	bool translate;

	FollowComponent(GameObject* subject, bool translate, bool rotate);

public:
	static FollowComponent* rotating(GameObject* subject);
	static FollowComponent* rotatingAndTranslating(GameObject* subject);
	static FollowComponent* translating(GameObject* subject);

	void onUpdate(float elapsedSeconds) override;
	FollowComponent* withOffset(vec3 offset);
};

class SmoothComponent : public GameObjectComponent
{
private:
	vec3 previousPosition;
	quat previousOrientation;
	bool rotate;
	bool translate;
	float snappyness;

	SmoothComponent(bool translate, bool rotate, float snappyness);

public:
	static SmoothComponent* rotating(float snappyness);
	static SmoothComponent* rotatingAndTranslating(float snappyness);
	static SmoothComponent* translating(float snappyness);

	void onAttach(GameObject* newParent) override;
	void onUpdate(float elapsedSeconds) override;
};
