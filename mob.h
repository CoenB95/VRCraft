#ifndef MOB_H
#define MOB_H

#include "camera.h"
#include "collisioncomponent.h"
#include "forcecomponent.h"
#include "gameobject.h"
#include "vec.h"
#include "world.h"

class MobEyes : public GameObject
{

};

class Mob : public GameObject
{
private:
	bool ceiled = false;
	
	World& world;
	BlockCollisionComponent collision;
	FloorCollisionComponent floorDetection;
	ForceComponent force;

public:
	const GLfloat mobHeight = 1.5f;
	const GLfloat mobDiameter = 0.30f;

	MobEyes eyes;

	Mob(World& world);
	void jump();
	void move(float angleDeg, float factor, float elapsedTime);
	void update(float elapsedSeconds) override;
};

class Steve : public Mob
{
public:
	Steve(World& world);
};

#endif // !MOB_H

