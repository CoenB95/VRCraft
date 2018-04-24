#include <GL/freeglut.h>
#include <cstddef>

#define STB_PERLIN_IMPLEMENTATION

#include "chunk.h"
#include "stb_perlin.h"

Chunk::Chunk(int width, int height, int depth)
{
	this->width = width;
	this->height = height;
	this->depth = depth;

	for (int y = 0; y < height; y++)
	{
		for (int z = -depth / 2; z < depth / 2; z++)
		{
			for (int x = -width / 2; x < width / 2; x++)
			{
				Block block(blockSize, blockSize, blockSize);
				block.isTransparent = stb_perlin_noise3((float)x / width, (float)y / height, (float)z / depth, 0, 0, 0) < 0.0f;
				block.setPosition(x * blockDrawSize, y * blockDrawSize, z * blockDrawSize);
				blocks.push_back(block);
			}
		}
	}

	blocks[1].setColor(Color4f::CYAN);
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
		blocks[i].drawRaw();
	}
}

Block* Chunk::getBlock(int x, int y, int z)
{
	int index = x + z * width + y * width * height;

	if (index < 0 || index >= width * height * depth)
		return nullptr;

	return &blocks[index];
}

void Chunk::update()
{
	if (blocksChanged)
	{
		blocksChanged = false;
		for (int i = 0; i < blocks.size(); i++)
		{
			blocks[i].backSide.shouldRender = i / width % depth == 0 ||
				(i - width) >= 0 && blocks[(i - width)].isTransparent;

			blocks[i].bottomSide.shouldRender = i / (width * depth) == 0 ||
				(i - width * depth) >= 0 && blocks[(i - width * depth)].isTransparent;

			blocks[i].frontSide.shouldRender = i / width % depth == depth - 1 ||
				(i + width) < width * depth * height && blocks[(i + width)].isTransparent;

			blocks[i].leftSide.shouldRender = i % width == 0 ||
				(i - 1) >= 0 && blocks[(i - 1)].isTransparent;

			blocks[i].rightSide.shouldRender = i % width == width - 1 ||
				(i + 1) < width * depth * height && blocks[(i + 1)].isTransparent;

			blocks[i].topSide.shouldRender = i / (width * depth) == height - 1 ||
				(i + width * depth) < width * depth * height && blocks[(i + width * depth)].isTransparent;
		}
	}
}
