#include "gamescene.h"

GameScene::GameScene() {

}

void GameScene::addObject(GameObject* object) {
	objects.push_back(object);
}