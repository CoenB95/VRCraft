#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION

#include <gl/glew.h>
#include <cstddef>
#include <iostream>
#include <vector>
#include <VrLib/Log.h>
#include <VrLib/Texture.h>

#define STB_PERLIN_IMPLEMENTATION

#include "block.h"
#include "block_grass.h"
#include "chunk.h"
#include "gameobject.h"
#include "stack.h"
#include "stb_perlin.h"
#include "texturedrawcomponent.h"

using vrlib::Log;
using vrlib::logger;

Chunk::Chunk(vec3 chunkSize, vec3 blockSize) : chunkSize(chunkSize), blockSize(blockSize) {
	for (int y = 0; y < chunkSize.y; y++)
	{
		for (int z = 0; z < chunkSize.z; z++)
		{
			for (int x = 0; x < chunkSize.x; x++)
			{
				Block* block;

				float noise = stb_perlin_noise3((float)x / chunkSize.x * 2 - 1, (float)y / chunkSize.y * 2 - 1, (float)z / chunkSize.z * 2 - 1, 0, 0, 0);
				if (noise > 0)
					block = new GrassBlock();
				else
					block = new AirBlock();

				block->position = vec3(x * blockSize.x, y * blockSize.y, z * blockSize.z);
				blocks.push_back(block);
			}
		}
	}
}

void Chunk::build(ChunkContext* chunkContext) {
	vertices.clear();
	vertices.reserve(blocks.size() * 36);
	for (GLuint i = 0; i < blocks.size(); i++) {
		if (blocks[i]->isDirty) {
			BlockContext context = getAdjacentBlocks(chunkContext, blocks[i]->position);
			blocks[i]->build(context);
		}
		vertices.insert(vertices.end(), blocks[i]->vertices.begin(), blocks[i]->vertices.end());
	}
}

Stack* Chunk::destroyBlock(Block* block)
{
	/*if (block == nullptr)
		return nullptr;

	Stack* dropped = new Stack(block, *this);
	notifyStackDropped(dropped);

	Block* newAirBlock = new AirBlock();
	newAirBlock->isTransparent = true;
	newAirBlock->position = block->position;
	notifyBlockChanged(newAirBlock);

	return dropped;*/
	return nullptr;
}

void Chunk::destroyStack(Stack* stack)
{
	notifyStackRemoved(stack);
}

BlockContext Chunk::getAdjacentBlocks(ChunkContext* chunkContext, vec3 positionInChunk) {
	Block* centerBlock = getBlock(positionInChunk);
	if (centerBlock == nullptr)
		return BlockContext(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

	BlockContext context = BlockContext();
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			for (int z = 0; z < 3; z++) {
				context.surroundings[x][y][z] = getBlock(centerBlock->position + vec3(x-1, y-1, z-1));
			}
		}
	}
	context.updateSides();
	/*getBlock(centerBlock->position + vec3(+0, +1, +0));
		getBlock(centerBlock->position + vec3(+0, +0, -1));
		getBlock(centerBlock->position + vec3(+1, +0, +0));
		getBlock(centerBlock->position + vec3(+0, +0, +1));
		getBlock(centerBlock->position + vec3(-1, +0, +0));
		getBlock(centerBlock->position + vec3(+0, -1, +0));*/

	if (context.top == nullptr && chunkContext->top != nullptr)
		context.top = chunkContext->top->getBlock(centerBlock->position + vec3(+0, -chunkSize.y + 1, +0));
	if (context.front == nullptr && chunkContext->front != nullptr)
		context.front = chunkContext->front->getBlock(centerBlock->position + vec3(+0, +0, +chunkSize.z - 1));
	if (context.right == nullptr && chunkContext->right != nullptr)
		context.right = chunkContext->right->getBlock(centerBlock->position + vec3(-chunkSize.x + 1, +0, +0));
	if (context.back == nullptr && chunkContext->back != nullptr)
		context.back = chunkContext->back->getBlock(centerBlock->position + vec3(+0, +0, -chunkSize.z + 1));
	if (context.left == nullptr && chunkContext->left != nullptr)
		context.left = chunkContext->left->getBlock(centerBlock->position + vec3(+chunkSize.x - 1, +0, +0));
	if (context.bottom == nullptr && chunkContext->bottom != nullptr)
		context.bottom = chunkContext->bottom->getBlock(centerBlock->position + vec3(+0, +chunkSize.y - 1, +0));

	return context;
}

Block* Chunk::getBlock(vec3 positionInChunk) {
	int index = getBlockIndex(positionInChunk);

	if (index < 0)
		return nullptr;

	return blocks[index];
}

int Chunk::getBlockIndex(vec3 positionInChunk)
{
	if (positionInChunk.x < 0 || positionInChunk.y < 0 || positionInChunk.z < 0)
		return -1;

	if (positionInChunk.x >= chunkSize.x || positionInChunk.y >= chunkSize.y || positionInChunk.z >= chunkSize.z)
		return -1;

	return (int)(roundf(positionInChunk.x) + roundf(positionInChunk.z) * chunkSize.x + roundf(positionInChunk.y) * chunkSize.x * chunkSize.z);
}

Block** Chunk::getBlockPtr(vec3 positionInChunk) {
	int index = getBlockIndex(positionInChunk);

	if (index < 0)
		return nullptr;

	return &blocks[index];
}

Stack* Chunk::getNearbyStack(vec3 position, float maxDistance)
{
	for (Stack* stack : items)
	{
		if ((position/*.distanceSquared(*/ - stack->position).length() < maxDistance)
			return stack;
	}
	return nullptr;
}

bool Chunk::isBlockTransparent(Block* block)
{
	return block != nullptr && block->isTransparent;
}

void Chunk::loadTextures() {
	TextureDrawComponent* component = new TextureDrawComponent("data/VrCraft/textures/terrain.png");
	component->verticesPtr = &vertices;
	addComponent(component);
}

Stack* Chunk::mergeStacks()
{
	for (GLuint s0 = 0; s0 < items.size(); s0++)
	{
		for (GLuint s1 = s0 + 1; s1 < items.size(); s1++)
		{
			if (items[s0]->getType()->getTypeName() == items[s1]->getType()->getTypeName() &&
				(items[s0]->position/*.distanceSquared*/ - (items[s1]->position)).length() < 1)
			{
				items[s0]->increaseStack(items[s1]->getStackSize());
				notifyStackRemoved(items[s1]);
				return items[s1];
			}
		}
	}
	return nullptr;
}

void Chunk::notifyBlockChanged(Block* newBlock)
{
	newBlocks.push_back(newBlock);
	blocksChanged = true;
}

void Chunk::notifyStackDropped(Stack* newStack)
{
	newItems.push_back(newStack);
	itemsChanged = true;
}

void Chunk::notifyStackRemoved(Stack* oldStack)
{
	removedItems.push_back(oldStack);
	itemsChanged = true;
}

void Chunk::randomTick(ChunkContext* chunkContext) {
	logger << "Ticking blocks..." << Log::newline;
	for (int i = 0; i < 5; i++) {
		int randomTickBlockIndex = rand() % blocks.size();
		Block** block = getBlockPtr(blocks[randomTickBlockIndex]->position);
		BlockContext blockContext = getAdjacentBlocks(chunkContext, (*block)->position);

		Block* newBlock = (*block)->randomTick(blockContext);
		if (newBlock != nullptr) {
			//notifyBlockChanged(newBlock);
			delete *block;
			*block = newBlock;
		}

		if ((*block)->isDirty) {
			(*block)->build(blockContext);
		}
	}

	logger << "Rebuilding chunk..." << Log::newline;
	build(chunkContext);
	logger << "Rebuilding chunk done." << Log::newline;
}

void Chunk::update(float elapsedSeconds) {
	GameObject::update(elapsedSeconds);

	if (blocksChanged)
	{
		blocksChanged = false;
		for (GLuint i = 0; i < newBlocks.size(); i++)
		{
			Block** block = getBlockPtr(newBlocks[i]->position);
			delete *block;
			*block = newBlocks[i];
		}
		newBlocks.clear();
	}

	if (itemsChanged)
	{
		itemsChanged = false;
		for (GLuint i = 0; i < removedItems.size(); i++)
		{
			vector<Stack*>::iterator it = find(items.begin(), items.end(), removedItems[i]);
			delete removedItems[i];
			items.erase(it);
		}
		removedItems.clear();

		for (GLuint i = 0; i < newItems.size(); i++)
		{
			items.push_back(newItems[i]);
		}
		newItems.clear();
	}
}
