#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <VrLib/gl/shader.h>

#include "shaders.h"

using namespace glm;
using namespace std;

class GameObjectComponent;

class GameObject
{
private:
	//DrawComponent* drawComponent = nullptr;
	vector<GameObjectComponent*> components;
	vrlib::gl::Shader<Shaders::Uniforms>* shader;

public:
	vec3 position;
	// The amount of rotation around the x-axis, in degrees.
	float rotateX = 0.0f;
	// The amount of rotation around the y-axis, in degrees.
	float rotateY = 0.0f;
	// The amount of rotation around the z-axis, in degrees.
	float rotateZ = 0.0f;

	GameObject();
	GameObject(GameObject& other);
	void addComponent(GameObjectComponent* component);
	virtual void draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix);
	void removeAllComponents();
	virtual void update(float elapsedSeconds);
};
