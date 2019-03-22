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

	PhysicsRigidBody* addShape(GameObject* object, PxShape* shaps, bool isStatic);

public:
	PhysicsRigidBody* addBox(GameObject* object, vec3 boxSize, bool isStatic) override;
	PhysicsRigidBody* addMesh(GameObject* object, bool isStatic) override;
	void onUpdate(float elapsedSeconds) override;
	void removeBody(PhysicsRigidBody* body) override;
	void setup(vec3 gravity) override;
};

class NvidiaBaseRigidBody : public PhysicsRigidBody {
public:
	PxRigidActor* actor;

	NvidiaBaseRigidBody(PxRigidActor* actor, NvidiaPhysics* world) : PhysicsRigidBody(world), actor(actor) {};

	vec3 getPosition() override;
	quat getOrientation() override;
};

class NvidiaDynamicRigidBody : public NvidiaBaseRigidBody {
public:
	PxRigidDynamic* dynamicActor;

	NvidiaDynamicRigidBody(PxRigidDynamic* actor, NvidiaPhysics* world)
		: NvidiaBaseRigidBody(actor, world), dynamicActor(actor) {};

	void addForce(vec3 force) override;
};

class NvidiaStaticRigidBody : public NvidiaBaseRigidBody {
public:
	PxRigidActor* staticActor;

	NvidiaStaticRigidBody(PxRigidActor* actor, NvidiaPhysics* world)
		: NvidiaBaseRigidBody(actor, world), staticActor(actor) {};

	void addForce(vec3 force) override {};
};