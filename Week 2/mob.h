#ifndef MOB_H
#define MOB_H

#include "camera.h"
#include "chunk.h"
#include "collisioncomponent.h"
#include "gameobject.h"
#include "vec.h"

class Mob : public GameObject
{
private:
	bool ceiled = false;
	bool floored = false;
	
	// TODO: replace with 'World' (multiple chunks)
	Chunk& world;
	BlockCollisionComponent* collision;

public:
	const GLfloat mobHeight = 1.5f;
	const GLfloat mobDiameter = 0.30f;

	float horizontalSensitivity = 0.1f;
	float horizontalSpeed = 0.0f;
	float verticalAcceleration = 25.0f;
	float verticalMaxSpeed = 25.0f;
	float verticalSpeed = 0.0f;
	Vec3f eyePosition;

	Mob(Chunk& world);
	bool isFloored() { return floored; };
	void move(float angleDeg, float factor, float elapsedTime);
	void update(float elapsedSeconds) override;
};

class Steve : public Mob
{
public:
	Steve(Chunk& world);
};

#endif // !MOB_H

