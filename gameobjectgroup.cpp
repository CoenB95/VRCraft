#include "gameobjectgroup.h"

using namespace std;

GameObjectGroup::GameObjectGroup() {

}

GameObjectGroup::~GameObjectGroup() {
	deleteAllChildren();
}

void GameObjectGroup::addChild(GameObject* object) {
	if (object == nullptr)
		return;

	lock_guard<mutex> lock(childrenMutex);
	object->parent = this;
	children.push_back(object);
}

void GameObjectGroup::build(vec3 offsetPosition) {
	GameObject::build(offsetPosition);
	for (GLuint i = 0; i < children.size(); i++) {
		if (children[i]->shouldRebuild()) {
			children[i]->buildStandalone();
		}
	}
};

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