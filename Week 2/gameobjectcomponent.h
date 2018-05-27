#pragma once

class GameObject;

class GameObjectComponent
{
protected:
	GameObject* parentObject;
public:
	GameObjectComponent();
	GameObjectComponent(GameObject* parent);
	inline void setParent(GameObject* object) { this->parentObject = object; }
	virtual void update(float elapsedSeconds) = 0;
};

class DrawComponent : public GameObjectComponent
{
public:
	DrawComponent();
	DrawComponent(GameObject* parent);
	virtual void draw() = 0;
};

class SpinComponent : public GameObjectComponent
{
private:
	float degreesPerSec;
	float value;
public:
	SpinComponent(float degreesPerSec);
	void update(float elapsedSeconds) override;
};
