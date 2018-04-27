#ifndef MOB_H
#define MOB_H

#include "camera.h"
#include "chunk.h"

class Mob
{
private:
	Camera eyes;
	GLfloat mobBlockHeight = 1.8f;
	// TODO: replace with 'World' (multiple chunks)
	Chunk& world;

	bool checkCollision(vector<Block::BlockContext> collisionBoxes, Block*(*f)(Block::BlockContext b));
public:
	Mob(Chunk& world);
	void move(float angleDeg, float factor, float elapsedTime);
	Camera& getEyes();
};

class Steve : public Mob
{
public:
	Steve(Chunk& world);
};

#endif // !MOB_H

