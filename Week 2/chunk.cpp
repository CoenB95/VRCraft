#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION

#include <GL/freeglut.h>
#include <cstddef>
#include <iostream>

#define STB_PERLIN_IMPLEMENTATION

#include "block_grass.h"
#include "chunk.h"
#include "stb_image.h"
#include "stb_perlin.h"

Chunk::Chunk(int width, int height, int depth)
{
	this->width = width;
	this->height = height;
	this->depth = depth;

	for (int y = 0; y < height; y++)
	{
		for (int z = 0; z < depth; z++)
		{
			for (int x = 0; x < width; x++)
			{
				Block* block = new GrassBlock();
				block->isTransparent = stb_perlin_noise3((float)x / width * 2 - 1, (float)y / height * 2 - 1, (float)z / depth * 2 - 1, 0, 0, 0) < 0.0f;
				block->position = Vec3f(x, y, z);
				blocks.push_back(block);
			}
		}
	}

	for (int i = 0; i < blocks.size(); i++)
	{
		Block::BlockContext context = getAdjacentBlocks(blocks[i]);
		
		Block* newBlock = blocks[i]->randomTick(context);
		if (newBlock != nullptr)
		{
			notifyBlockChanged(newBlock);
		}
	}

	addComponent(new ChunkDrawComponent());
}

Stack* Chunk::destroyBlock(Block* block)
{
	if (block == nullptr)
		return nullptr;

	Stack* dropped = new Stack(block, *this);
	notifyStackDropped(dropped);

	Block* newAirBlock = new AirBlock();
	newAirBlock->isTransparent = true;
	newAirBlock->position = block->position;
	notifyBlockChanged(newAirBlock);

	return dropped;
}

void Chunk::destroyStack(Stack* stack)
{
	notifyStackRemoved(stack);
}

void Chunk::drawRaw()
{
	for (int i = 0; i < blocks.size(); i++)
	{
		blocks[i]->drawRaw();
	}
}

Block::BlockContext Chunk::getAdjacentBlocks(Block* base)
{
	int i = getBlockIndex(base);
	Block::BlockContext context = Block::BlockContext(
		getBlock(blocks[i]->position.x, blocks[i]->position.y + 1, blocks[i]->position.z),
		getBlock(blocks[i]->position.x, blocks[i]->position.y, blocks[i]->position.z - 1),
		getBlock(blocks[i]->position.x + 1, blocks[i]->position.y, blocks[i]->position.z),
		getBlock(blocks[i]->position.x, blocks[i]->position.y, blocks[i]->position.z + 1),
		getBlock(blocks[i]->position.x - 1, blocks[i]->position.y, blocks[i]->position.z),
		getBlock(blocks[i]->position.x, blocks[i]->position.y - 1, blocks[i]->position.z)
	);
	return context;
}

Block* Chunk::getBlock(int index)
{
	if (index < 0 || index >= blocks.size())
		return nullptr;

	return blocks[index];
}

Block* Chunk::getBlock(float x, float y, float z)
{
	return getBlock((int)x, (int)y, (int)z);
}

Block* Chunk::getBlock(int x, int y, int z)
{
	Block** ptr = getBlockPtr(x, y, z);
	return (ptr == nullptr ? nullptr : *ptr);
}

int Chunk::getBlockIndex(Block* block)
{
	return getBlockIndex((int)block->position.x, (int)block->position.y, (int)block->position.z);
}

int Chunk::getBlockIndex(int x, int y, int z)
{
	if (x < 0 || x >= width || y < 0 || y >= height || z < 0 || z >= depth)
		return -1;

	return x + z * width + y * width * depth;
}

Block** Chunk::getBlockPtr(int x, int y, int z)
{
	int index = getBlockIndex(x, y, z);

	if (index < 0 || index >= width * height * depth)
		return nullptr;

	return &blocks[index];
}

Stack* Chunk::getNearbyStack(Vec3f position, float maxDistance)
{
	for (Stack* stack : items)
	{
		if (position.distanceSquared(stack->position) < maxDistance)
			return stack;
	}
	return nullptr;
}

bool Chunk::isBlockTransparent(Block* block)
{
	return block != nullptr && block->isTransparent;
}

Stack* Chunk::mergeStacks()
{
	for (int s0 = 0; s0 < items.size(); s0++)
	{
		for (int s1 = s0 + 1; s1 < items.size(); s1++)
		{
			if (items[s0]->getType()->getTypeName() == items[s1]->getType()->getTypeName() &&
				items[s0]->position.distanceSquared(items[s1]->position) < 1)
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

void Chunk::update(float elapsedSeconds)
{
	GameObject::update(elapsedSeconds);

	if (blocksChanged)
	{
		blocksChanged = false;
		for (int i = 0; i < newBlocks.size(); i++)
		{
			Block** block = getBlockPtr((int)newBlocks[i]->position.x, (int)newBlocks[i]->position.y, (int)newBlocks[i]->position.z);
			delete *block;
			*block = newBlocks[i];
		}
		newBlocks.clear();

		for (int i = 0; i < blocks.size(); i++)
		{
			Block::BlockContext context = getAdjacentBlocks(blocks[i]);
			blocks[i]->topSide->shouldRender = context.top == nullptr || context.top->isTransparent;
			blocks[i]->frontSide->shouldRender = context.front == nullptr || context.front->isTransparent;
			blocks[i]->rightSide->shouldRender = context.right == nullptr || context.right->isTransparent;
			blocks[i]->backSide->shouldRender = context.back == nullptr || context.back->isTransparent;
			blocks[i]->leftSide->shouldRender = context.left == nullptr || context.left->isTransparent;
			blocks[i]->bottomSide->shouldRender = context.bottom == nullptr || context.bottom->isTransparent;
		}
	}

	if (itemsChanged)
	{
		itemsChanged = false;
		for (int i = 0; i < removedItems.size(); i++)
		{
			vector<Stack*>::iterator it = find(items.begin(), items.end(), removedItems[i]);
			delete removedItems[i];
			items.erase(it);
		}
		removedItems.clear();

		for (int i = 0; i < newItems.size(); i++)
		{
			items.push_back(newItems[i]);
		}
		newItems.clear();
	}
}

ChunkDrawComponent::ChunkDrawComponent() : DrawComponent()
{

}

GLuint ChunkDrawComponent::terrainTextureId = -1;

void ChunkDrawComponent::draw()
{
	if (terrainTextureId < 0)
		return;

	Chunk* chunk = dynamic_cast<Chunk*>(parentObject);
	if (chunk == nullptr)
		return;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, terrainTextureId);

	glBegin(GL_TRIANGLES);
	chunk->drawRaw();
	glEnd();
}

void ChunkDrawComponent::loadTextures()
{
	cout << "Loading textures... " << endl;

	int imageWidth, imageHeight, imageComponents;
	stbi_uc* image = stbi_load("terrain.png", &imageWidth, &imageHeight, &imageComponents, 0);

	if (image == nullptr)
	{
		cout << "Could not load textures" << endl << "  Reason: " << stbi_failure_reason() << endl;
	}
	else
	{
		cout << "Image size: " << imageWidth << "x" << imageHeight << endl;

		glGenTextures(1, &terrainTextureId);
		glBindTexture(GL_TEXTURE_2D, terrainTextureId);

		glTexImage2D(GL_TEXTURE_2D,
			0,					//level
			GL_RGBA,			//internal format
			imageWidth,			//width
			imageHeight,		//height
			0,					//border
			GL_RGBA,			//data format
			GL_UNSIGNED_BYTE,	//data type
			image);				//data

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glEnable(GL_TEXTURE_2D);

		stbi_image_free(image);
	}

	cout << "Loading textures done" << endl;
}
