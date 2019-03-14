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

void GameObject::build() {
	dirty = false;
}

mat4 GameObject::calcModelMatrix(const mat4& parentModelMatrix) {
	mat4 modelMatrix = parentModelMatrix;
	modelMatrix = glm::translate(modelMatrix, position * vec3(1.01f, 1.01f, -1.01f));
	modelMatrix = glm::scale(modelMatrix, scale);
	modelMatrix = glm::translate(modelMatrix, verticesOffset);
	modelMatrix *= glm::toMat4(orientation);
	modelMatrix = glm::translate(modelMatrix, -verticesOffset);
	return modelMatrix;
}

void GameObject::draw(const mat4& projectionMatrix, const mat4& viewMatrix, const mat4& parentModelMatrix) {
	mat4 modelMatrix = calcModelMatrix(parentModelMatrix);

	Shaders::useShader(shader, projectionMatrix, viewMatrix, modelMatrix);

	for (GameObjectComponent* component : components)
		component->onDraw(projectionMatrix, viewMatrix, modelMatrix);
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
