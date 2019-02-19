#pragma once

#include <glm/glm.hpp>
#include <vector>

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
	// The amount of rotation around the x-axis, in degrees.
	float rotateX = 0.0f;
	// The amount of rotation around the y-axis, in degrees.
	float rotateY = 0.0f;
	// The amount of rotation around the z-axis, in degrees.
	float rotateZ = 0.0f;

	GameObject();
	GameObject(GameObject& other);
	void addComponent(GameObjectComponent* component);
	void draw();
	void removeAllComponents();
	virtual void update(float elapsedSeconds);
};
