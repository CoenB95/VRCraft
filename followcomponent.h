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
	float snappyness;

	FollowComponent(GameObject* subject, bool translate, bool rotate, float snappyness);

public:
	static FollowComponent* rotating(GameObject* subject, float snappyness = 1.0f);
	static FollowComponent* rotatingAndTranslating(GameObject* subject, float snappyness = 1.0f);
	static FollowComponent* translating(GameObject* subject, float snappyness = 1.0f);

	void onUpdate(float elapsedSeconds) override;
	FollowComponent* withOffset(vec3 offset);
};

class SmoothComponent : public GameObjectComponent
{
private:
	vec3 prev;
	float prevRotX;
	float prevRotY;
	float prevRotZ;
	bool rotate;
	bool translate;
	float snappyness;

	SmoothComponent(bool translate, bool rotate, float snappyness);

public:
	static SmoothComponent* rotating(float snappyness);
	static SmoothComponent* rotatingAndTranslating(float snappyness);
	static SmoothComponent* translating(float snappyness);

	void onUpdate(float elapsedSeconds) override;
};
