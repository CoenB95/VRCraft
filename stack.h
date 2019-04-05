#pragma once

#include "gameobjectgroup.h"

class Block;
class World;

class Stack : public GameObjectGroup {
private:
	Block* blockType;
	int maxStackSize = 64;
	int stackSize = 1;

public:
	Stack(Block* type, World* world, int amount = 1);

	int adjustCount(int delta);
	inline int getStackSize() { return stackSize; };
	inline Block* getType() { return blockType; };
	void setCount(int value);
};
