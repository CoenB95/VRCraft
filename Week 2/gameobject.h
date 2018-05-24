#pragma once

#include <vector>

#include "vec.h"

using namespace std;

class DrawComponent;
class GameObjectComponent;

class GameObject
{
private:
	DrawComponent* drawComponent;
	vector<GameObjectComponent*> components;
public:
	Vec3f position;
	// The amount of rotation around the x-axis, in degrees.
	float rotateX;
	// The amount of rotation around the y-axis, in degrees.
	float rotateY;
	// The amount of rotation around the z-axis, in degrees.
	float rotateZ;

	GameObject();
	void addComponent(GameObjectComponent* component);
	void draw();
	virtual void update(float elapsedSeconds);
};
