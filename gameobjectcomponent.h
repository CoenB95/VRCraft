#pragma once

#include "gameobject.h"

class GameObjectComponent
{
protected:
	GameObject* parentObject;

public:
	GameObjectComponent();

	inline virtual void onAttach(GameObject* newParent) {};
	inline virtual void onDraw(const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix, const glm::mat4& modelMatrix) {};
	inline virtual void onUpdate(float elapsedSeconds) {};
	void setParent(GameObject* object);
};

class SpinComponent : public GameObjectComponent
{
private:
	float degreesPerSec;
	float value;
public:
	SpinComponent(float degreesPerSec);
	void onUpdate(float elapsedSeconds) override;
};
