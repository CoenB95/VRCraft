#include "collisioncomponent.h"
#include "chunk.h"
#include "followcomponent.h"
#include "forcecomponent.h"
#include "stack.h"

Stack::Stack(Block* type, Chunk& world) : GameObject()
{
	blockType = new Block(*type);
	blockType->setScale(Block::SCALE_ITEM);

	position = blockType->position;
	position += Vec3f(0, 1, 0);

	//Note to self: child has relative translation.
	addComponent(new ChildDrawComponent(blockType));
	blockType->position = Vec3f(0.0f, Block::SCALE_ITEM / 2, 0.0f);

	addComponent(new SpinComponent(50.0f));
	addComponent(new SimpleGravityComponent());
	addComponent(new FloorCollisionComponent(world));
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

ChildDrawComponent::ChildDrawComponent(GameObject* child) : DrawComponent(), child(child)
{

}

void ChildDrawComponent::draw()
{
	if (child != nullptr)
		child->draw();
}

void ChildDrawComponent::update(float elapsedSeconds)
{
	if (child != nullptr)
		child->update(elapsedSeconds);
}
