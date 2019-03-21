#pragma once

#include "gameobjectcomponent.h"

class PhysicsRigidBody;

class PhysicsComponent : public GameObjectComponent {
private:
	PhysicsRigidBody* rigidBody;

public:
	PhysicsComponent(PhysicsRigidBody* rigidBody);

	void onAttach(GameObject* newParent) override;
	void onUpdate(float elapsedSeconds) override;
};