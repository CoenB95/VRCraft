#include <GL/freeglut.h>

#include "chunk.h"

Chunk::Chunk(int width, int depth)
{
	this->width = width;
	this->depth = depth;

	for (int x = -width / 2; x < width / 2; x++)
	{
		for (int z = -depth / 2; z < depth / 2; z++)
		{
			Block block(1.0f, 1.0f, 1.0f);
			block.setPosition(x + x * 0.1f, -1.0f, z + z * 0.1f);
			blocks.push_back(block);
		}
	}
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
		blocks[i].drawRaw();
	}
}
