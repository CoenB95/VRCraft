#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
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

public:
	vec3 position;
	quat orientation;

	vrlib::gl::Shader<Shaders::Uniforms>* shader;

	GameObject();
	GameObject(GameObject& other);
	void addComponent(GameObjectComponent* component);
	virtual void draw(const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix);
	void removeAllComponents();
	virtual void update(float elapsedSeconds);
};
