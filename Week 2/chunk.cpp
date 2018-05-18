#include <GL/freeglut.h>
#include <cstddef>
#include <iostream>

#define STB_PERLIN_IMPLEMENTATION

#include "block_grass.h"
#include "chunk.h"
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
				block->pos.set(x, y, z);
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
}

void Chunk::draw()
{
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);

	glBegin(GL_TRIANGLES);
	drawRaw();
	glEnd();

	glPopMatrix();
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
		getBlock(blocks[i]->pos.x, blocks[i]->pos.y + 1, blocks[i]->pos.z),
		getBlock(blocks[i]->pos.x, blocks[i]->pos.y, blocks[i]->pos.z - 1),
		getBlock(blocks[i]->pos.x + 1, blocks[i]->pos.y, blocks[i]->pos.z),
		getBlock(blocks[i]->pos.x, blocks[i]->pos.y, blocks[i]->pos.z + 1),
		getBlock(blocks[i]->pos.x - 1, blocks[i]->pos.y, blocks[i]->pos.z),
		getBlock(blocks[i]->pos.x, blocks[i]->pos.y - 1, blocks[i]->pos.z)
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
	return getBlockIndex((int)block->pos.x, (int)block->pos.y, (int)block->pos.z);
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

bool Chunk::isBlockTransparent(Block* block)
{
	return block != nullptr && block->isTransparent;
}

void Chunk::notifyBlockChanged(Block* newBlock)
{
	newBlocks.push_back(newBlock);
	blocksChanged = true;
}

void Chunk::update()
{
	if (blocksChanged)
	{
		blocksChanged = false;
		for (int i = 0; i < newBlocks.size(); i++)
		{
			Block** block = getBlockPtr((int)newBlocks[i]->pos.x, (int)newBlocks[i]->pos.y, (int)newBlocks[i]->pos.z);
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
}
