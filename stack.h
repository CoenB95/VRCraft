#pragma once

#include "gameobjectcomponent.h"

class Block;
class World;

class Stack : public GameObject {
private:
	Block* blockType;
	int maxStackSize = 64;
	int stackSize = 1;

public:
	Stack(Block* type, World& world);
	int decreaseStack(int amount = 1);
	inline int getStackSize() { return stackSize; };
	inline Block* getType() { return blockType; };
	int increaseStack(int amount = 1);
};
