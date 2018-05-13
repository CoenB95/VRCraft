#ifndef MOB_H
#define MOB_H

#include "camera.h"
#include "chunk.h"
#include "vec.h"

class Mob
{
private:
	Camera eyes;
	bool floored = false;
	Block* lastFloor;
	GLfloat mobBlockHeight = 1.5f;
	GLfloat mobDiameter = 0.25f;
	// TODO: replace with 'World' (multiple chunks)
	Chunk& world;

	bool checkCollision(vector<Block::BlockContext> collisionBoxes, Block*(*f)(Block::BlockContext b));
public:
	float const accelY = 25.0f;
	float speedY = 0;

	Mob(Chunk& world);
	Camera& getCamera();
	Vec3f getEyePos();
	GLfloat getMobHeight() const { return mobBlockHeight; }
	bool isFloored() { return floored; };
	void move(float angleDeg, float factor, float elapsedTime);
	void update(float elapsedTime);
};

class Steve : public Mob
{
public:
	Steve(Chunk& world);
};

#endif // !MOB_H

