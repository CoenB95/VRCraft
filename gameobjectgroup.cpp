#include "gameobjectgroup.h"

using namespace std;

GameObjectGroup::GameObjectGroup() {

}

GameObjectGroup::~GameObjectGroup() {
	deleteAllChildren();
}

void GameObjectGroup::addChild(GameObject* object) {
	lock_guard<mutex> lock(childrenMutex);
	children.push_back(object);
}

void GameObjectGroup::deleteAllChildren() {
	lock_guard<mutex> lock(childrenMutex);
	for (GameObject* child : children) {
		delete child;
	}
	children.clear();
}

void GameObjectGroup::deleteChild(GameObject* object) {
	if (object == nullptr)
		return;

	lock_guard<mutex> lock(childrenMutex);
	vector<GameObject*>::iterator it = find(children.begin(), children.end(), object);
	if (it != children.end()) {
		children.erase(it);
		delete object;
	}
}

void GameObjectGroup::draw(const mat4& parentModelMatrix) {
	GameObject::draw(parentModelMatrix);

	if (children.empty())
		return;

	mat4 modelMatrix = calcModelMatrix(parentModelMatrix);
	vector<GameObject*> childrenCopy;
	{
		lock_guard<mutex> lock(childrenMutex);
		childrenCopy = children;
	}
	for (GameObject* child : childrenCopy) {
		child->draw(modelMatrix);
	}
}

void GameObjectGroup::update(float elapsedSeconds) {
	GameObject::update(elapsedSeconds);

	if (children.empty())
		return;

	//Iterate on a copy so that objects can remove themselves.
	vector<GameObject*> childrenCopy;
	{
		lock_guard<mutex> lock(childrenMutex);
		childrenCopy = children;
	}
	for (GameObject* child : childrenCopy) {
		child->update(elapsedSeconds);
	}
}

void GameObjectGroup::removeChild(GameObject* object) {
	if (object == nullptr)
		return;

	lock_guard<mutex> lock(childrenMutex);
	vector<GameObject*>::iterator it = find(children.begin(), children.end(), object);
	if (it != children.end())
		children.erase(it);
}