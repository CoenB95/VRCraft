#pragma once

#include "gameobjectcomponent.h"

class ForceComponent : public GameObjectComponent
{
private:
	vec3 curForce;
	vec3 newForce;

public:
	ForceComponent();
	void addForce(vec3 value);
	void clearForces();
	void onUpdate(float elapsedSeconds) override;
};

class SimpleGravityComponent : public ForceComponent
{
public:
	SimpleGravityComponent();
	void onUpdate(float elapsedSeconds) override;
};
