#pragma once

class World;

class MobEyes : public GameObject
{

};

class Mob : public GameObject
{
private:
	bool ceiled = false;
	
	World& world;

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
