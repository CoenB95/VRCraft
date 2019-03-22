#pragma once

#include "gameobjectcomponent.h"

class PhysicsRigidBody;

class PhysicsComponent : public GameObjectComponent {
public:
	PhysicsRigidBody* rigidBody;

	PhysicsComponent(PhysicsRigidBody* rigidBody, string tag = "");
	~PhysicsComponent();

	void onAttach(GameObject* newParent) override;
	void onUpdate(float elapsedSeconds) override;
};