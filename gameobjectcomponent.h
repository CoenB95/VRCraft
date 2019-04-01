#pragma once

#include "gameobject.h"

class GameObjectComponent {
private:
	string tag;

protected:
	GameObject* parentObject;

public:
	GameObjectComponent(string tag = "");
	virtual ~GameObjectComponent() {};

	inline string getTag() { return tag; };
	inline virtual void onAttach(GameObject* newParent) {};
	inline virtual void onBuild(vec3 offset) {};
	inline virtual void onDraw(const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix, const glm::mat4& modelMatrix) {};
	inline virtual void onUpdate(float elapsedSeconds) {};
	void setParent(GameObject* object);
};

class SpinComponent : public GameObjectComponent {
private:
	float degreesPerSec;
	float value;
public:
	SpinComponent(float degreesPerSec, string tag = "");
	void onUpdate(float elapsedSeconds) override;
};
