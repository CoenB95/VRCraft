#include "physicscomponent.h"

#include "physics.h"

PhysicsComponent::PhysicsComponent(PhysicsRigidBody* rigidBody, string tag) : GameObjectComponent(tag) {
	this->rigidBody = rigidBody;
}

void PhysicsComponent::onAttach(GameObject* newParent) {
	
}

void PhysicsComponent::onUpdate(float elapsedSeconds) {
	parentObject->position = rigidBody->getPosition();
	parentObject->orientation = rigidBody->getOrientation();
}