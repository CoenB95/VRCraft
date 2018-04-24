#include <GL/freeglut.h>

#include "chunk.h"

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
				Block block(1.0f, 1.0f, 1.0f);
				block.setPosition(x + x * 0.1f, y + y * 0.1, z + z * 0.1f);
				blocks.push_back(block);
			}
		}
	}

	blocks[1].setColor(Color4f::CYAN);
}

void Chunk::draw()
{
	glPushMatrix();
	glTranslatef((float)width / 2, 0.0f, (float)depth / 2);

	glBegin(GL_TRIANGLES);
	drawRaw();
	glEnd();

	glPopMatrix();
}

void Chunk::drawRaw()
{
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

		blocks[i].drawRaw();
	}
}
