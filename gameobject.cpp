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

GameObject::GameObject(GameObject& other) {
	position = other.position;
	orientation = other.orientation;
}

void GameObject::addComponent(GameObjectComponent* component) {
	component->setParent(this);

	this->components.push_back(component);
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

void GameObject::draw(const mat4& projectionMatrix, const mat4& viewMatrix, const mat4& parentModelMatrix) {
	mat4 modelMatrix = calcModelMatrix(parentModelMatrix);

	Shaders::useShader(shader, projectionMatrix, viewMatrix, modelMatrix);

	for (GameObjectComponent* component : components)
		component->onDraw(projectionMatrix, viewMatrix, modelMatrix);
}

GameObjectComponent* GameObject::getComponent(string tag) {
	if (tag.empty())
		return nullptr;

	for (GameObjectComponent* component : components) {
		if (component->getTag() == tag)
			return component;
	}

	return nullptr;
}

void GameObject::removeAllComponents() {
	for (GameObjectComponent* component : components)
	{
		if (component != nullptr)
			delete component;
	}

	components.clear();
}

void GameObject::update(float elapsedSeconds) {
	for (GameObjectComponent* component : components)
		component->onUpdate(elapsedSeconds);
}
