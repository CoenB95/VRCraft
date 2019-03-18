#ifndef _DEBUG
#pragma comment(lib, "PhysX3CHECKED_x86.lib")
#pragma comment(lib, "PhysX3CookingCHECKED_x86.lib")
#pragma comment(lib, "PhysX3CommonCHECKED_x86.lib")
#pragma comment(lib, "PhysX3CharacterKinematicCHECKED_x86.lib")
#pragma comment(lib, "PhysX3VehicleCHECKED.lib")
#pragma comment(lib, "PhysX3ExtensionsCHECKED.lib")
#pragma comment(lib, "PxTaskCHECKED_x86.lib")
#pragma comment(lib, "PxPvdSDKCHECKED_x86.lib")
#pragma comment(lib, "PxFoundationCHECKED_x86.lib")
#pragma comment(lib, "PxPvdSDKCHECKED_x86.lib")
#pragma comment(lib, "PxTaskCHECKED_x86.lib")
#else
#pragma comment(lib, "PhysX3DEBUG_x86.lib")
#pragma comment(lib, "PhysX3CookingDEBUG_x86.lib")
#pragma comment(lib, "PhysX3CommonDEBUG_x86.lib")
#pragma comment(lib, "PhysX3CharacterKinematicDEBUG_x86.lib")
#pragma comment(lib, "PhysX3VehicleDEBUG.lib")
#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")
#pragma comment(lib, "PxTaskDEBUG_x86.lib")
#pragma comment(lib, "PxPvdSDKDEBUG_x86.lib")
#pragma comment(lib, "PxFoundationDEBUG_x86.lib")
#pragma comment(lib, "PxPvdSDKDEBUG_x86.lib")
#pragma comment(lib, "PxTaskDEBUG_x86.lib")
#endif

#include "physics_nvidia.h"

void NvidiaPhysics::setup(vec3 gravity) {
	pxFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, pxAllocator, pxErrorCallback);

	pxSdk = PxCreatePhysics(PX_PHYSICS_VERSION, *pxFoundation, PxTolerancesScale(), true);

	pxDispatcher = PxDefaultCpuDispatcherCreate(2);

	PxSceneDesc sceneDescriptor(pxSdk->getTolerancesScale());
	sceneDescriptor.cpuDispatcher = pxDispatcher;
	sceneDescriptor.filterShader = PxDefaultSimulationFilterShader;
	sceneDescriptor.gravity = PxVec3(gravity.x, gravity.y, gravity.z);
	pxWorld = pxSdk->createScene(sceneDescriptor);



	PxTriangleMeshDesc meshDescriptor;
}