#pragma once

#include <glm/glm.hpp>

#include "gameobject.h"

using namespace std;
using namespace glm;

class PhysicsRigidBody;

typedef function<void(PhysicsRigidBody*)> PhysicsCollisionCallback;

class PhysicsWorld {
public:
	virtual PhysicsRigidBody* addBox(GameObject* object, vec3 boxSize, bool isStatic = false) = 0;
	virtual PhysicsRigidBody* addMesh(GameObject* object, bool isStatic = false) = 0;
	virtual void onUpdate(float elapsedSeconds) {};
	virtual void removeBody(PhysicsRigidBody* body) = 0;
	virtual void setup(vec3 gravity) = 0;
};

class PhysicsRigidBody {
private:
	PhysicsCollisionCallback collisionCallback;
	GameObject* object;
	PhysicsWorld* parentPhysicsWorld;

public:
	PhysicsRigidBody(GameObject* object, PhysicsWorld* parentPhysicsWorld)
		: object(object), parentPhysicsWorld(parentPhysicsWorld) {};

	virtual void addForce(vec3 force) = 0;
	virtual vec3 getPosition() = 0;
	virtual quat getOrientation() = 0;

	PhysicsCollisionCallback getCollisionListener() { return collisionCallback; };
	GameObject* getObject() { return object; };
	void removeFromWorld() { parentPhysicsWorld->removeBody(this); };
	void setCollisionListener(PhysicsCollisionCallback callback) { collisionCallback = callback; }
};