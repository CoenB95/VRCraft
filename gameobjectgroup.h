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
	void deleteAllChildren();
	void deleteChild(GameObject* object);
	void draw(const mat4& parentModelMatrix) override;
	void update(float elapsedSeconds) override;
	void removeChild(GameObject* object);
};