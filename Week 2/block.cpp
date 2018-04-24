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

void Block::draw() {
	GLfloat hw = width / 2;
	GLfloat hh = height / 2;
	GLfloat hd = depth / 2;

	glTranslatef((float)x, 0.0f, (float)y);

	glBegin(GL_TRIANGLES);

	// Front
	glColor4fv(frontColor.color4fv);
	glVertex3f(-hw, hh, hd);
	glVertex3f(-hw, -hh, hd);
	glVertex3f(hw, -hh, hd);
	glVertex3f(hw, hh, hd);
	glVertex3f(-hw, hh, hd);
	glVertex3f(hw, -hh, hd);

	// Top
	glColor4fv(topColor.color4fv);
	glVertex3f(-hw, hh, -hd);
	glVertex3f(-hw, hh, hd);
	glVertex3f(hw, hh, hd);
	glVertex3f(hw, hh, -hd);
	glVertex3f(-hw, hh, -hd);
	glVertex3f(hw, hh, hd);

	// Right
	glColor4fv(rightColor.color4fv);
	glVertex3f(hw, hh, hd);
	glVertex3f(hw, -hh, hd);
	glVertex3f(hw, -hh, -hd);
	glVertex3f(hw, hh, -hd);
	glVertex3f(hw, hh, hd);
	glVertex3f(hw, -hh, -hd);

	// Back
	glColor4fv(backColor.color4fv);
	glVertex3f(hw, hh, -hd);
	glVertex3f(hw, -hh, -hd);
	glVertex3f(-hw, -hh, -hd);
	glVertex3f(-hw, hh, -hd);
	glVertex3f(hw, hh, -hd);
	glVertex3f(-hw, -hh, -hd);

	// Bottom
	glColor4fv(bottomColor.color4fv);
	glVertex3f(-hw, -hh, hd);
	glVertex3f(-hw, -hh, -hd);
	glVertex3f(hw, -hh, -hd);
	glVertex3f(hw, -hh, hd);
	glVertex3f(-hw, -hh, hd);
	glVertex3f(hw, -hh, -hd);

	// Left
	glColor4fv(leftColor.color4fv);
	glVertex3f(-hw, hh, -hd);
	glVertex3f(-hw, -hh, -hd);
	glVertex3f(-hw, -hh, hd);
	glVertex3f(-hw, hh, hd);
	glVertex3f(-hw, hh, -hd);
	glVertex3f(-hw, -hh, hd);

	glEnd();
}

void Block::setPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}
