#pragma once

#include <vector>

#include "vec.h"

using namespace std;

class DrawComponent;
class GameObjectComponent;

class GameObject
{
private:
	DrawComponent* drawComponent = nullptr;
	vector<GameObjectComponent*> components;

public:
	Vec3f position;
	// The amount of rotation around the x-axis, in degrees.
	float rotateX = 0.0f;
	// The amount of rotation around the y-axis, in degrees.
	float rotateY = 0.0f;
	// The amount of rotation around the z-axis, in degrees.
	float rotateZ = 0.0f;

	GameObject();
	void addComponent(GameObjectComponent* component);
	void draw();
	virtual void update(float elapsedSeconds);
};
