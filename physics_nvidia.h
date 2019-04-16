#pragma once

#include <PxPhysicsAPI.h>
#include <glm/glm.hpp>

#include "physics.h"

using namespace glm;
using namespace physx;

class GameObject;
class NvidiaBaseRigidBody;
class NvidiaEventCallback;

class NvidiaPhysics : public PhysicsWorld, public PxSimulationEventCallback {
private:
	vector<NvidiaBaseRigidBody*> bodies;
	map<NvidiaBaseRigidBody*, NvidiaBaseRigidBody*> collisionsToCall;
	float elapsedSimulationSeconds = 0;

	PxDefaultAllocator pxAllocator;
	PxDefaultErrorCallback pxErrorCallback;
	
	PxCooking* pxCooking = nullptr;
	PxMaterial* pxDefaultMaterial = nullptr;
	PxDefaultCpuDispatcher* pxDispatcher = nullptr;
	PxFoundation* pxFoundation = nullptr;
	PxPvd* pxPvd = nullptr;
	PxPhysics* pxSdk = nullptr;

	PxScene* pxWorld = nullptr;

	PhysicsRigidBody* addShape(GameObject* object, PxShape* shaps, bool isStatic);

public:
	PhysicsRigidBody* addBox(GameObject* object, vec3 boxSize, bool isStatic) override;
	PhysicsRigidBody* addMesh(GameObject* object, bool isStatic) override;
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override;
	void onUpdate(float elapsedSeconds) override;
	void removeBody(PhysicsRigidBody* body) override;
	void setup(vec3 gravity) override;

	void onConstraintBreak(PxConstraintInfo * constraints, PxU32 count) override {};
	void onWake(PxActor ** actors, PxU32 count) override {};
	void onSleep(PxActor ** actors, PxU32 count) override {};
	void onTrigger(PxTriggerPair * pairs, PxU32 count) override {};
	void onAdvance(const PxRigidBody * const * bodyBuffer, const PxTransform * poseBuffer, const PxU32 count) override {};
};

class NvidiaBaseRigidBody : public PhysicsRigidBody {
public:
	PxRigidActor* actor;

	NvidiaBaseRigidBody(PxRigidActor* actor, GameObject* object, NvidiaPhysics* world)
		: PhysicsRigidBody(object, world), actor(actor) {};

	vec3 getPosition() override;
	quat getOrientation() override;
};

class NvidiaDynamicRigidBody : public NvidiaBaseRigidBody {
public:
	PxRigidDynamic* dynamicActor;

	NvidiaDynamicRigidBody(PxRigidDynamic* actor, GameObject* object, NvidiaPhysics* world)
		: NvidiaBaseRigidBody(actor, object, world), dynamicActor(actor) {};

	void addForce(vec3 force) override;
};

class NvidiaStaticRigidBody : public NvidiaBaseRigidBody {
public:
	PxRigidActor* staticActor;

	NvidiaStaticRigidBody(PxRigidActor* actor, GameObject* object, NvidiaPhysics* world)
		: NvidiaBaseRigidBody(actor, object, world), staticActor(actor) {};

	void addForce(vec3 force) override {};
};