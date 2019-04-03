#include "gameobject.h"
#include "gameobjectcomponent.h"

GameObjectComponent::GameObjectComponent(string tag) : tag(tag) {

}

void GameObjectComponent::setParent(GameObject* newParent) {
	parentObject = newParent;
	onAttach(parentObject);
}


