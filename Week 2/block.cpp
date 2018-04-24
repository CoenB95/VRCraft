#include <GL/freeglut.h>

#include "block.h"

Block::Block() :
	frontColor(Color4f::RED),
	topColor(Color4f::BLUE),
	rightColor(Color4f::GREEN),
	backColor(Color4f::YELLOW),
	bottomColor(Color4f::CYAN),
	leftColor(Color4f::MAGENTA)
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
	if (isAir)
		return;

	GLfloat hw = width / 2;
	GLfloat hh = height / 2;
	GLfloat hd = depth / 2;

	// Front
	glColor4fv(frontColor.color4fv);
	glVertex3f(x - hw, y + hh, z + hd);
	glVertex3f(x - hw, y - hh, z + hd);
	glVertex3f(x + hw, y - hh, z + hd);
	glVertex3f(x + hw, y + hh, z + hd);
	glVertex3f(x - hw, y + hh, z + hd);
	glVertex3f(x + hw, y - hh, z + hd);

	// Top
	glColor4fv(topColor.color4fv);
	glVertex3f(x - hw, y + hh, z - hd);
	glVertex3f(x - hw, y + hh, z + hd);
	glVertex3f(x + hw, y + hh, z + hd);
	glVertex3f(x + hw, y + hh, z - hd);
	glVertex3f(x - hw, y + hh, z - hd);
	glVertex3f(x + hw, y + hh, z + hd);

	// Right
	glColor4fv(rightColor.color4fv);
	glVertex3f(x + hw, y + hh, z + hd);
	glVertex3f(x + hw, y - hh, z + hd);
	glVertex3f(x + hw, y - hh, z - hd);
	glVertex3f(x + hw, y + hh, z - hd);
	glVertex3f(x + hw, y + hh, z + hd);
	glVertex3f(x + hw, y - hh, z - hd);

	// Back
	glColor4fv(backColor.color4fv);
	glVertex3f(x + hw, y + hh, z - hd);
	glVertex3f(x + hw, y - hh, z - hd);
	glVertex3f(x - hw, y - hh, z - hd);
	glVertex3f(x - hw, y + hh, z - hd);
	glVertex3f(x + hw, y + hh, z - hd);
	glVertex3f(x - hw, y - hh, z - hd);

	// Bottom
	glColor4fv(bottomColor.color4fv);
	glVertex3f(x - hw, y - hh, z + hd);
	glVertex3f(x - hw, y - hh, z - hd);
	glVertex3f(x + hw, y - hh, z - hd);
	glVertex3f(x + hw, y - hh, z + hd);
	glVertex3f(x - hw, y - hh, z + hd);
	glVertex3f(x + hw, y - hh, z - hd);

	// Left
	glColor4fv(leftColor.color4fv);
	glVertex3f(x - hw, y + hh, z - hd);
	glVertex3f(x - hw, y - hh, z - hd);
	glVertex3f(x - hw, y - hh, z + hd);
	glVertex3f(x - hw, y + hh, z + hd);
	glVertex3f(x - hw, y + hh, z - hd);
	glVertex3f(x - hw, y - hh, z + hd);
}

void Block::setPosition(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
