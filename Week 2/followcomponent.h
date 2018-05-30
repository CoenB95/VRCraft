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
	static FollowComponent* rotating(GameObject* subject, float snappyness = 1.0f);
	static FollowComponent* rotatingAndTranslating(GameObject* subject, float snappyness = 1.0f);
	static FollowComponent* translating(GameObject* subject, float snappyness = 1.0f);

	void update(float elapsedSeconds) override;
	FollowComponent* withOffset(Vec3f offset);
};

class SmoothComponent : public GameObjectComponent
{
private:
	Vec3f prev;
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

	void update(float elapsedSeconds) override;
};
