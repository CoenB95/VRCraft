#pragma once

#include <glm/glm.hpp>

#include "gameobject.h"

using namespace glm;

class PhysicsRigidBody;

class PhysicsWorld {
public:
	//virtual PhysicsRigidBody* addBox() = 0;
	virtual PhysicsRigidBody* addMesh(GameObject* object, bool isStatic = false) = 0;
	virtual void onUpdate(float elapsedSeconds) {};
	virtual void setup(vec3 gravity) = 0;
};

class PhysicsRigidBody {
public:
	virtual void addForce(vec3 force) = 0;
	virtual vec3 getPosition() = 0;
	virtual quat getOrientation() = 0;
};