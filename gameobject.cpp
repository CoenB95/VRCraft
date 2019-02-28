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

void GameObject::draw(const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix) {
	glm::mat4 modelMatrix = glm::mat4();
	modelMatrix = glm::translate(modelMatrix, position * vec3(1.01f, 1.01f, -1.01f));
	modelMatrix *= glm::toMat4(orientation);

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
