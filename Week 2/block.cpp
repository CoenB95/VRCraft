#include <GL/freeglut.h>

#include "block.h"

Block::Block() :
	frontSide(Color4f::RED),
	topSide(Color4f::BLUE),
	rightSide(Color4f::GREEN),
	backSide(Color4f::YELLOW),
	bottomSide(Color4f::CYAN),
	leftSide(Color4f::MAGENTA)
{
	this->x = 0;
	this->y = 0;
	this->width = 1;
	this->height = 0;
	this->depth = 0;
}

Block::Block(float w, float h, float d) : Block()
{
	this->width = w;
	this->height = h;
	this->depth = d;
}

void Block::draw()
{
	glTranslatef((float)x, 0.0f, (float)y);

	glBegin(GL_TRIANGLES);
	drawRaw();
	glEnd();
}

void Block::drawRaw()
{
	if (isTransparent)
		return;

	GLfloat hw = width / 2;
	GLfloat hh = height / 2;
	GLfloat hd = depth / 2;

	if (frontSide.shouldRender)
	{
		frontSide.applyTexture();
		glVertex3f(x - hw, y + hh, z + hd);
		glVertex3f(x - hw, y - hh, z + hd);
		glVertex3f(x + hw, y - hh, z + hd);
		glVertex3f(x + hw, y + hh, z + hd);
		glVertex3f(x - hw, y + hh, z + hd);
		glVertex3f(x + hw, y - hh, z + hd);
	}

	if (topSide.shouldRender)
	{
		topSide.applyTexture();
		glVertex3f(x - hw, y + hh, z - hd);
		glVertex3f(x - hw, y + hh, z + hd);
		glVertex3f(x + hw, y + hh, z + hd);
		glVertex3f(x + hw, y + hh, z - hd);
		glVertex3f(x - hw, y + hh, z - hd);
		glVertex3f(x + hw, y + hh, z + hd);
	}

	if (rightSide.shouldRender)
	{
		rightSide.applyTexture();
		glVertex3f(x + hw, y + hh, z + hd);
		glVertex3f(x + hw, y - hh, z + hd);
		glVertex3f(x + hw, y - hh, z - hd);
		glVertex3f(x + hw, y + hh, z - hd);
		glVertex3f(x + hw, y + hh, z + hd);
		glVertex3f(x + hw, y - hh, z - hd);
	}

	if (backSide.shouldRender)
	{
		backSide.applyTexture();
		glVertex3f(x + hw, y + hh, z - hd);
		glVertex3f(x + hw, y - hh, z - hd);
		glVertex3f(x - hw, y - hh, z - hd);
		glVertex3f(x - hw, y + hh, z - hd);
		glVertex3f(x + hw, y + hh, z - hd);
		glVertex3f(x - hw, y - hh, z - hd);
	}

	if (bottomSide.shouldRender)
	{
		bottomSide.applyTexture();
		glVertex3f(x - hw, y - hh, z + hd);
		glVertex3f(x - hw, y - hh, z - hd);
		glVertex3f(x + hw, y - hh, z - hd);
		glVertex3f(x + hw, y - hh, z + hd);
		glVertex3f(x - hw, y - hh, z + hd);
		glVertex3f(x + hw, y - hh, z - hd);
	}

	
	if (leftSide.shouldRender)
	{
		leftSide.applyTexture();
		glVertex3f(x - hw, y + hh, z - hd);
		glVertex3f(x - hw, y - hh, z - hd);
		glVertex3f(x - hw, y - hh, z + hd);
		glVertex3f(x - hw, y + hh, z + hd);
		glVertex3f(x - hw, y + hh, z - hd);
		glVertex3f(x - hw, y - hh, z + hd);
	}
}

void Block::setPosition(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

//======================

BlockSide::BlockSide(Color4f color) : color(color)
{

}

void BlockSide::applyTexture()
{
	glColor4fv(color.color4fv);
}
