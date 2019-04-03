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
	inline virtual void onDraw() {};
	inline virtual void onUpdate(float elapsedSeconds) {};
	void setParent(GameObject* object);
};