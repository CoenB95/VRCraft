#pragma once

#include "gameobject.h"

class GameScene {
private:
	vector<GameObject*> objects;

public:
	GameScene();
	void addObject(GameObject* object);
};