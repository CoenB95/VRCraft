#include "block.h"
#include "followcomponent.h"
#include "gameobject.h"
#include "spincomponent.h"
#include "stack.h"
#include "world.h"

Stack::Stack(Block* type, World& world) : GameObject() {
	blockType = new Block();
	//blockType->setScale(Block::SCALE_ITEM);

	position = blockType->position;
	position += vec3(0, 1, 0);

	//Note to self: child has relative translation.
	//addComponent(new ChildDrawComponent(blockType));
	blockType->position = vec3(0.0f, Block::SCALE_ITEM / 2, 0.0f);

	addComponent(new SpinComponent(50.0f));
}

int Stack::increaseStack(int amount)
{
	int newSize = stackSize + amount;
	if (newSize > maxStackSize)
		newSize = maxStackSize;

	int added = newSize - stackSize;
	stackSize = newSize;
	return added;
}

int Stack::decreaseStack(int amount)
{
	int newSize = stackSize - amount;
	if (newSize < 0)
		newSize = 0;

	int removed = stackSize - newSize;
	stackSize = newSize;
	return removed;
}
