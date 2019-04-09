#pragma once

#include <mutex>
#include <vector>

#include "gameobject.h"

using namespace std;

class GameObjectGroup : public GameObject {
private:
	vector<GameObject*> children;
	mutex childrenMutex;

public:
	GameObjectGroup();
	~GameObjectGroup();

	void addChild(GameObject* object);
	virtual void build(vec3 offset) override;
	void deleteAllChildren();
	void deleteChild(GameObject* object);
	virtual void draw(const mat4& parentModelMatrix) override;
	virtual void update(float elapsedSeconds) override;
	void removeChild(GameObject* object);
};