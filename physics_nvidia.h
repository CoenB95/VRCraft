#pragma once

#include <PxPhysicsAPI.h>
#include <glm/glm.hpp>

#include "physics.h"

using namespace glm;
using namespace physx;

class GameObject;

class NvidiaPhysics : public PhysicsWorld {
private:
	PxDefaultAllocator pxAllocator;
	PxDefaultErrorCallback pxErrorCallback;
	
	PxCooking* pxCooking;
	PxMaterial* pxDefaultMaterial;
	PxDefaultCpuDispatcher* pxDispatcher;
	PxFoundation* pxFoundation;
	PxPvd* pxPvd;
	PxPhysics* pxSdk;

	PxScene* pxWorld;

public:
	PhysicsRigidBody* addMesh(GameObject* object, bool isStatic) override;
	void onUpdate(float elapsedSeconds) override;
	void setup(vec3 gravity) override;
};

class NvidiaRigidBody : public PhysicsRigidBody {
private:
	PxRigidActor* actor;

public:
	NvidiaRigidBody(PxRigidActor* actor);

	void addForce(vec3 force) override;
	vec3 getPosition() override;
	quat getOrientation() override;
};