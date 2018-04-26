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
				block->setPosition(x, y, z);
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
			newBlocks.push_back(newBlock);
			blocksChanged = true;
		}
	}
}

void Chunk::draw()
{
	glPushMatrix();
	glTranslatef((float)0.0f, -1.0f + blockDrawSize / 2, 0.0f);

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

Block::BlockContext& Chunk::getAdjacentBlocks(Block* base)
{
	int i = getBlockIndex(base);
	Block::BlockContext context = Block::BlockContext(
		getBlock(blocks[i]->x, blocks[i]->y + 1, blocks[i]->z),
		getBlock(blocks[i]->x, blocks[i]->y, blocks[i]->z + 1),
		getBlock(blocks[i]->x + 1, blocks[i]->y, blocks[i]->z),
		getBlock(blocks[i]->x, blocks[i]->y, blocks[i]->z - 1),
		getBlock(blocks[i]->x - 1, blocks[i]->y, blocks[i]->z),
		getBlock(blocks[i]->x, blocks[i]->y - 1, blocks[i]->z)
	);
	return context;
}

Block* Chunk::getBlock(int index)
{
	if (index < 0 || index >= blocks.size())
		return nullptr;

	return blocks[index];
}

Block* Chunk::getBlock(int x, int y, int z)
{
	Block** ptr = getBlockPtr(x, y, z);
	return (ptr == nullptr ? nullptr : *ptr);
}

int Chunk::getBlockIndex(Block* block)
{
	return getBlockIndex(block->x, block->y, block->z);
}

int Chunk::getBlockIndex(int x, int y, int z)
{
	return x + z * width + y * width * height;
}

Block** Chunk::getBlockPtr(int x, int y, int z)
{
	int index = getBlockIndex(x, y, z);

	if (index < 0 || index >= width * height * depth)
		return nullptr;

	return &blocks[index];
}

void Chunk::notifyBlockChanged(Block* newBlock)
{
	newBlocks.push_back(newBlock);
	blocksChanged = true;
	delete newBlock;
}

void Chunk::update()
{
	if (blocksChanged)
	{
		blocksChanged = false;
		for (int i = 0; i < newBlocks.size(); i++)
		{
			Block** block = getBlockPtr(newBlocks[i]->x, newBlocks[i]->y, newBlocks[i]->z);
			delete *block;
			*block = newBlocks[i];
		}
		newBlocks.clear();

		for (int i = 0; i < blocks.size(); i++)
		{
			blocks[i]->backSide->shouldRender = i / width % depth == 0 ||
				(i - width) >= 0 && blocks[(i - width)]->isTransparent;

			blocks[i]->bottomSide->shouldRender = i / (width * depth) == 0 ||
				(i - width * depth) >= 0 && blocks[(i - width * depth)]->isTransparent;

			blocks[i]->frontSide->shouldRender = i / width % depth == depth - 1 ||
				(i + width) < width * depth * height && blocks[(i + width)]->isTransparent;

			blocks[i]->leftSide->shouldRender = i % width == 0 ||
				(i - 1) >= 0 && blocks[(i - 1)]->isTransparent;

			blocks[i]->rightSide->shouldRender = i % width == width - 1 ||
				(i + 1) < width * depth * height && blocks[(i + 1)]->isTransparent;

			blocks[i]->topSide->shouldRender = i / (width * depth) == height - 1 ||
				(i + width * depth) < width * depth * height && blocks[(i + width * depth)]->isTransparent;
		}
	}
}
