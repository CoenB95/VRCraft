#pragma once

#include "gameobjectcomponent.h"

class PhysicsWorld;
class PhysicsRigidBody;

enum ShapeType {
	BOX, MESH
};

class PhysicsComponent : public GameObjectComponent {
private:
	PhysicsRigidBody* rigidBody;
	PhysicsWorld* world;
	ShapeType type;
	bool isStatic;
	vec3 boxSize;

	void buildPhysics();
	void deletePhysics();

public:
	PhysicsComponent(PhysicsWorld* world, ShapeType type, bool isStatic, vec3 boxSize = vec3(1,1,1), string tag = "");
	~PhysicsComponent();

	PhysicsRigidBody* getBody() { return rigidBody; };
	void onBuild(vec3 offset) override;
	void onUpdate(float elapsedSeconds) override;
};