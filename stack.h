#pragma once

#include "block.h"

class Chunk;

class Stack : public GameObject
{
private:
	Block* blockType;
	int maxStackSize = 64;
	int stackSize = 1;

public:
	Stack(Block* type, Chunk& world);
	int decreaseStack(int amount = 1);
	inline int getStackSize() { return stackSize; };
	inline Block* getType() { return blockType; };
	int increaseStack(int amount = 1);
};

class ChildDrawComponent : public DrawComponent
{
private:
	GameObject* child;

public:
	ChildDrawComponent(GameObject* child);
	void draw() override;
	void update(float elapsedSeconds) override;
};
