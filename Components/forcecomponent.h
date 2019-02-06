#pragma once

#include "gameobjectcomponent.h"
#include "vec.h"

class ForceComponent : public GameObjectComponent
{
private:
	Vec3f curForce;
	Vec3f newForce;

public:
	ForceComponent();
	void addForce(Vec3f value);
	void clearForces();
	void update(float elapsedSeconds) override;
};

class SimpleGravityComponent : public ForceComponent
{
public:
	SimpleGravityComponent();
	void update(float elapsedSeconds) override;
};
