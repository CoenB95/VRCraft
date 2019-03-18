#pragma once

#include <PxPhysicsAPI.h>
#include <glm/glm.hpp>

using namespace glm;
using namespace physx;

class NvidiaPhysics {
private:
	PxDefaultAllocator pxAllocator;
	PxDefaultErrorCallback pxErrorCallback;
	
	PxDefaultCpuDispatcher* pxDispatcher;
	PxFoundation* pxFoundation;
	PxPhysics* pxSdk;

	PxScene* pxWorld;

public:
	void setup(vec3 gravity);
};