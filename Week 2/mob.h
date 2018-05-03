#ifndef MOB_H
#define MOB_H

#include "camera.h"
#include "chunk.h"

class Mob
{
private:
	Camera eyes;
	bool floored = false;
	Block* lastFloor;
	GLfloat mobBlockHeight = 1.0f;
	GLfloat mobDiameter = 0.25f;
	// TODO: replace with 'World' (multiple chunks)
	Chunk& world;

	bool checkCollision(vector<Block::BlockContext> collisionBoxes, Block*(*f)(Block::BlockContext b));
public:
	Mob(Chunk& world);
	Camera& getEyes();
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

