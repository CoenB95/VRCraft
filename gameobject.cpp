#include <iostream>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "gameobject.h"
#include "gameobjectcomponent.h"
#include "shaders.h"

using namespace std;

GameObject::GameObject() {
	shader = Shaders::DEFAULT_SHADER;
}

GameObject::GameObject(GameObject& other) {
	position = other.position;
	rotateX = other.rotateX;
	rotateY = other.rotateY;
	rotateZ = other.rotateZ;
}

void GameObject::addComponent(GameObjectComponent* component) {
	component->setParent(this);

	this->components.push_back(component);
}

void GameObject::draw(const glm::mat4& projectionMatrix, const glm::mat4& modelViewMatrix) {
	glm::mat4 modelMatrix = glm::mat4();
	modelMatrix = glm::translate(modelMatrix, position * vec3(1.01f, 1.01f, -1.01f));

	Shaders::useShader(shader, projectionMatrix, modelViewMatrix);
	/*shader->use();
	shader->setUniform(Shaders::Uniforms::projectionMatrix, projectionMatrix);
	shader->setUniform(Shaders::Uniforms::viewMatrix, modelViewMatrix);
	shader->setUniform(Shaders::Uniforms::modelMatrix, modelMatrix);
	shader->setUniform(Shaders::Uniforms::diffuseColor, glm::vec4(1, 1, 1, 1));
	shader->setUniform(Shaders::Uniforms::textureFactor, 1.0f);*/

	for (GameObjectComponent* component : components)
		component->onDraw(projectionMatrix, modelViewMatrix);
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
