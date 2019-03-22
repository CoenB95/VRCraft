#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "gameobject.h"
#include "gameobjectcomponent.h"
#include "shaders.h"

using namespace std;

GameObject::GameObject() {
	shader = Shaders::DEFAULT_SHADER;
}

GameObject::~GameObject() {
	deleteAllComponents();
}

GameObject::GameObject(GameObject& other) {
	position = other.position;
	orientation = other.orientation;
}

void GameObject::addComponent(GameObjectComponent* component) {
	component->setParent(this);
	lock_guard<mutex> lock(componentsMutex);
	components.push_back(component);
}

void GameObject::build(vec3 offset) {
	dirty = false;
}

void GameObject::buildEmbedded(vec3 offset) {
	build(offset + position);
}

void GameObject::buildStandalone(bool pivotAsCenter) {
	build(pivotAsCenter ? -pivot : vec3(0, 0, 0));
}

mat4 GameObject::calcModelMatrix(const mat4& parentModelMatrix) {
	mat4 modelMatrix = parentModelMatrix;
	modelMatrix = glm::translate(modelMatrix, position * vec3(1.0f, 1.0f, 1.0f));
	modelMatrix = glm::scale(modelMatrix, scale);
	modelMatrix *= glm::toMat4(orientation);
	return modelMatrix;
}

void GameObject::deleteAllComponents() {
	lock_guard<mutex> lock(componentsMutex);
	for (GameObjectComponent* component : components) {
		delete component;
	}
	components.clear();
}

void GameObject::deleteComponent(GameObjectComponent* component) {
	if (component == nullptr)
		return;

	lock_guard<mutex> lock(componentsMutex);
	vector<GameObjectComponent*>::iterator it = find(components.begin(), components.end(), component);
	if (it != components.end()) {
		components.erase(it);
		delete component;
	}
}

void GameObject::draw(const mat4& projectionMatrix, const mat4& viewMatrix, const mat4& parentModelMatrix) {
	if (components.empty())
		return;

	mat4 modelMatrix = calcModelMatrix(parentModelMatrix);

	Shaders::useShader(shader, projectionMatrix, viewMatrix, modelMatrix);

	vector<GameObjectComponent*> componentsCopy;
	{
		lock_guard<mutex> lock(componentsMutex);
		componentsCopy = components;
	}
	for (GameObjectComponent* component : componentsCopy) {
		component->onDraw(projectionMatrix, viewMatrix, modelMatrix);
	}
}

GameObjectComponent* GameObject::findComponentByTag(string tag) {
	if (tag.empty())
		return nullptr;

	lock_guard<mutex> lock(componentsMutex);
	for (GameObjectComponent* component : components) {
		if (component->getTag() == tag)
			return component;
	}

	return nullptr;
}

void GameObject::removeComponent(GameObjectComponent* component) {
	if (component == nullptr)
		return;

	lock_guard<mutex> lock(componentsMutex);
	vector<GameObjectComponent*>::iterator it = find(components.begin(), components.end(), component);
	if (it != components.end()) {
		components.erase(it);
		component->setParent(nullptr);
	}
}

void GameObject::update(float elapsedSeconds) {
	if (components.empty())
		return;

	//Iterate on a copy so that components can remove themselves.
	vector<GameObjectComponent*> componentsCopy;
	{
		lock_guard<mutex> lock(componentsMutex);
		componentsCopy = components;
	}
	for (GameObjectComponent* component : componentsCopy) {
		component->onUpdate(elapsedSeconds);
	}
}
