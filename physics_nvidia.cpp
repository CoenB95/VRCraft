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

PhysicsRigidBody* NvidiaPhysics::addBox(GameObject* object, vec3 boxSize, bool isStatic) {
	if (pxWorld == nullptr)
		return nullptr;

	PxShape* shape = pxSdk->createShape(PxBoxGeometry(boxSize.x / 2, boxSize.y / 2, boxSize.z / 2), *pxDefaultMaterial);
	return addShape(object, shape, isStatic);
}

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
	return addShape(object, shape, isStatic);
}

PxFilterFlags SampleSubmarineFilterShader(
	PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	/*// let triggers through
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}
	// generate contacts for all that were not filtered above*/
	pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1)) {
		pairFlags |= PxPairFlag::eCONTACT_EVENT_POSE;
		pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
	}
		
	return PxFilterFlag::eDEFAULT;
}

PhysicsRigidBody* NvidiaPhysics::addShape(GameObject* object, PxShape* shape, bool isStatic) {
	PxVec3 initialPosition(object->position.x, object->position.y, object->position.z);
	PxQuat initialOrientation(object->orientation.x, object->orientation.y, object->orientation.z, object->orientation.w);
	PxTransform initialTransform(initialPosition, initialOrientation);

	PxRigidActor* actor;
	NvidiaBaseRigidBody* body;
	if (isStatic) {
		actor = pxSdk->createRigidStatic(initialTransform);
		body = new NvidiaStaticRigidBody(actor, object, this);
	} else {
		PxRigidDynamic* rigidActor;
		actor = rigidActor = pxSdk->createRigidDynamic(initialTransform);
		body = new NvidiaDynamicRigidBody(rigidActor, object, this);
		PxRigidBodyExt::updateMassAndInertia(*rigidActor, 1.0f);
	}

	//PxSetGroup(*actor, isStatic ? 1 : 2);
	//PxSetGroupCollisionFlag(1, 2, true);

	PxFilterData dat;
	dat.word0 = isStatic ? 1 : 2;
	dat.word1 = isStatic ? 2 : 1;
	shape->setSimulationFilterData(dat);
	actor->attachShape(*shape);
	shape->release();
	pxWorld->addActor(*actor);
	bodies.push_back(body);

	return body;
}

void NvidiaPhysics::onUpdate(float elapsedSeconds) {
	if (pxWorld == nullptr)
		return;

	elapsedSimulationSeconds += elapsedSeconds;

	while (elapsedSimulationSeconds > 0.010f) {
		pxWorld->simulate(0.010f);
		pxWorld->fetchResults(true);
		elapsedSimulationSeconds -= 0.010f;
	}

	for (auto collision : collisionsToCall) {
		if (collision.first->getCollisionListener() != nullptr)
			(collision.first->getCollisionListener())(collision.second);
	}
	collisionsToCall.clear();
}

void NvidiaPhysics::removeBody(PhysicsRigidBody* body) {
	NvidiaBaseRigidBody* nvidiaBody = dynamic_cast<NvidiaBaseRigidBody*>(body);
	if (nvidiaBody == nullptr)
		return;

	vector<NvidiaBaseRigidBody*>::iterator it = find(bodies.begin(), bodies.end(), nvidiaBody);
	if (it != bodies.end()) {
		bodies.erase(it);
		pxWorld->removeActor(*nvidiaBody->actor);
	}
}

void NvidiaPhysics::setup(vec3 gravity) {
	pxFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, pxAllocator, pxErrorCallback);

	pxPvd = PxCreatePvd(*pxFoundation);
	PxPvdTransport* pvdTransport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
	pxPvd->connect(*pvdTransport, PxPvdInstrumentationFlag::eALL);

	pxSdk = PxCreatePhysics(PX_PHYSICS_VERSION, *pxFoundation, PxTolerancesScale(), true, pxPvd);

	pxCooking = PxCreateCooking(PX_PHYSICS_VERSION, *pxFoundation, PxCookingParams(pxSdk->getTolerancesScale()));

	pxDefaultMaterial = pxSdk->createMaterial(0.2f, 0.2f, 0.0f);
	pxDispatcher = PxDefaultCpuDispatcherCreate(2);

	PxSceneDesc sceneDescriptor(pxSdk->getTolerancesScale());
	sceneDescriptor.cpuDispatcher = pxDispatcher;
	sceneDescriptor.filterShader = SampleSubmarineFilterShader; //PxDefaultSimulationFilterShader;
	sceneDescriptor.gravity = PxVec3(gravity.x, gravity.y, gravity.z);
	sceneDescriptor.simulationEventCallback = this;
	pxWorld = pxSdk->createScene(sceneDescriptor);

	PxPvdSceneClient* pvdClient = pxWorld->getScenePvdClient();
	if (pvdClient) {
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
}

void NvidiaPhysics::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) {
	for (PxU32 i = 0; i < nbPairs; i++) {
		const PxContactPair& cp = pairs[i];

		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND) {
			NvidiaBaseRigidBody* body1 = nullptr;
			NvidiaBaseRigidBody* body2 = nullptr;
			for (GLuint i = 0; i < bodies.size(); i++) {
				if (bodies[i]->actor == pairHeader.actors[0]) {
					body1 = bodies[i];
				} else if (bodies[i]->actor == pairHeader.actors[1]) {
					body2 = bodies[i];
				}
			}

			if (body1 == nullptr || body2 == nullptr)
				return;

			if (body1->getCollisionListener() != nullptr) {
				collisionsToCall[body1] = body2;
				//(body1->getCollisionListener())(body2);
			} else if (body2->getCollisionListener() != nullptr) {
				collisionsToCall[body2] = body1;
				//(body2->getCollisionListener())(body1);
			}
		}
	}
}

vec3 NvidiaBaseRigidBody::getPosition() {
	PxVec3 pos = actor->getGlobalPose().p;
	return vec3(pos.x, pos.y, pos.z);
}

quat NvidiaBaseRigidBody::getOrientation() {
	PxQuat orien = actor->getGlobalPose().q;
	return quat(orien.w, orien.x, orien.y, orien.z);
}

void NvidiaDynamicRigidBody::addForce(vec3 force) {
	dynamicActor->addForce(PxVec3(force.x, force.y, force.z));
}