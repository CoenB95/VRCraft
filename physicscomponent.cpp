#include "physicscomponent.h"

#include "physics.h"

PhysicsComponent::PhysicsComponent(PhysicsWorld* world, ShapeType type, bool isStatic, vec3 boxSize, string tag) : GameObjectComponent(tag) {
	this->world = world;
	this->type = type;
	this->isStatic = isStatic;
	this->boxSize = boxSize;
}

PhysicsComponent::~PhysicsComponent() {
	deletePhysics();
}

void PhysicsComponent::buildPhysics() {
	if (rigidBody != nullptr)
		deletePhysics();

	switch (type) {
	case BOX:
		this->rigidBody = world->addBox(parentObject, boxSize, isStatic);
		break;
	case MESH:
		this->rigidBody = world->addMesh(parentObject, isStatic);
		break;
	default:
		break;
	}
}

void PhysicsComponent::deletePhysics() {
	if (rigidBody == nullptr)
		return;

	rigidBody->removeFromWorld();
	delete rigidBody;
	rigidBody = nullptr;
}

void PhysicsComponent::onBuild(vec3 offset) {
	buildPhysics();
}

void PhysicsComponent::onUpdate(float elapsedSeconds) {
	if (rigidBody == nullptr)
		return;

	parentObject->position = rigidBody->getPosition();
	parentObject->orientation = rigidBody->getOrientation();
}