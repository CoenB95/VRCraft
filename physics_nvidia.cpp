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

PhysicsRigidBody* NvidiaPhysics::addMesh(GameObject* object, bool isStatic) {
	if (pxWorld == nullptr || object->vertices.empty())
		return nullptr;

	lock_guard<mutex> lock(object->verticesMutex);

	PxVec3* pointsData = new PxVec3[object->vertices.size()];
	PxU32* trianglesData = new PxU32[object->vertices.size()];

	for (size_t i = 0; i < object->vertices.size(); i++) {
		pointsData[i] = PxVec3(object->vertices[i].px, object->vertices[i].py, object->vertices[i].pz);
		trianglesData[i] = i;
	}

	PxTriangleMeshDesc meshDescriptor;
	meshDescriptor.points.count = object->vertices.size();
	meshDescriptor.points.stride = sizeof(PxVec3);
	meshDescriptor.points.data = pointsData;
	meshDescriptor.triangles.count = object->vertices.size() / 3;
	meshDescriptor.triangles.stride = 3 * sizeof(PxU32);
	meshDescriptor.triangles.data = trianglesData;

	PxDefaultMemoryOutputStream meshOutputStream;
	if (!pxCooking->cookTriangleMesh(meshDescriptor, meshOutputStream))
		throw "uh oh";

	PxDefaultMemoryInputData meshInputStream(meshOutputStream.getData(), meshOutputStream.getSize());
	PxShape* shape = pxSdk->createShape(PxTriangleMeshGeometry(pxSdk->createTriangleMesh(meshInputStream)), *pxDefaultMaterial);

	PxVec3 initialPosition(object->position.x, object->position.y, object->position.z);
	PxQuat initialOrientation(object->orientation.x, object->orientation.y, object->orientation.z, object->orientation.w);
	PxTransform initialTransform(initialPosition, initialOrientation);

	PxRigidActor* actor;
	if (isStatic) {
		actor = pxSdk->createRigidStatic(initialTransform);
	} else {
		PxRigidDynamic* rigidActor;
		actor = rigidActor = pxSdk->createRigidDynamic(initialTransform);
		//rigidActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		PxRigidBodyExt::updateMassAndInertia(*rigidActor, 1.0f);
	}
	actor->attachShape(*shape);
	pxWorld->addActor(*actor);

	return new NvidiaRigidBody(actor);
}

void NvidiaPhysics::onUpdate(float elapsedSeconds) {
	if (pxWorld == nullptr)
		return;

	pxWorld->simulate(0.013f);
	pxWorld->fetchResults(true);
}

void NvidiaPhysics::setup(vec3 gravity) {
	pxFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, pxAllocator, pxErrorCallback);

	pxPvd = PxCreatePvd(*pxFoundation);
	PxPvdTransport* pvdTransport = PxDefaultPvdSocketTransportCreate("145.48.205.28", 5425, 10);
	pxPvd->connect(*pvdTransport, PxPvdInstrumentationFlag::eALL);

	pxSdk = PxCreatePhysics(PX_PHYSICS_VERSION, *pxFoundation, PxTolerancesScale(), true, pxPvd);

	pxCooking = PxCreateCooking(PX_PHYSICS_VERSION, *pxFoundation, PxCookingParams(pxSdk->getTolerancesScale()));

	pxDefaultMaterial = pxSdk->createMaterial(0.2f, 0.2f, 0.0f);
	pxDispatcher = PxDefaultCpuDispatcherCreate(2);

	PxSceneDesc sceneDescriptor(pxSdk->getTolerancesScale());
	sceneDescriptor.cpuDispatcher = pxDispatcher;
	sceneDescriptor.filterShader = PxDefaultSimulationFilterShader;
	sceneDescriptor.gravity = PxVec3(gravity.x, gravity.y, gravity.z);
	pxWorld = pxSdk->createScene(sceneDescriptor);

	PxPvdSceneClient* pvdClient = pxWorld->getScenePvdClient();
	if (pvdClient) {
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
}

NvidiaRigidBody::NvidiaRigidBody(PxRigidActor* actor) : actor(actor) {

}

void NvidiaRigidBody::addForce(vec3 force) {
	PxRigidDynamic* dynamicActor = dynamic_cast<PxRigidDynamic*>(actor);
	if (dynamicActor == nullptr)
		return;

	dynamicActor->addForce(PxVec3(force.x, force.y, force.z));
}

vec3 NvidiaRigidBody::getPosition() {
	PxVec3 pos = actor->getGlobalPose().p;
	return vec3(pos.x, pos.y, pos.z);
}

quat NvidiaRigidBody::getOrientation() {
	PxQuat orien = actor->getGlobalPose().q;
	return quat(orien.x, orien.y, orien.z, orien.w);
}