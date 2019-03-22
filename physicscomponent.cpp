#include "physicscomponent.h"

#include "physics.h"

PhysicsComponent::PhysicsComponent(PhysicsRigidBody* rigidBody, string tag) : GameObjectComponent(tag) {
	this->rigidBody = rigidBody;
}

PhysicsComponent::~PhysicsComponent() {
	rigidBody->removeFromWorld();
	delete rigidBody;
}

void PhysicsComponent::onAttach(GameObject* newParent) {
	
}

void PhysicsComponent::onUpdate(float elapsedSeconds) {
	parentObject->position = rigidBody->getPosition();
	parentObject->orientation = rigidBody->getOrientation();
}