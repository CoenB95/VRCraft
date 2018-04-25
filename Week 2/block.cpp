#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES

#include <GL/freeglut.h>
#include <cmath>

#include "block.h"

Block::Block() :
	frontSide(new TexturedBlockSide(0, 0, 1.0f / 256.0f * 16.0f, 1.0f / 256.0f * 16.0f)),
	topSide(new SimpleBlockSide(Color4f::BLUE)),
	rightSide(new SimpleBlockSide(Color4f::GREEN)),
	backSide(new SimpleBlockSide(Color4f::YELLOW)),
	bottomSide(new SimpleBlockSide(Color4f::CYAN)),
	leftSide(new SimpleBlockSide(Color4f::MAGENTA))
{
	this->x = 0;
	this->y = 0;
	this->hw = 0.5f;
	this->hh = 0.5f;
	this->hd = 0.5f;
}

Block::Block(float w, float h, float d) : Block()
{
	this->hw = w / 2;
	this->hh = h / 2;
	this->hd = d / 2;
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

	if (frontSide->shouldRender)
	{
		drawVertex(frontSide, x - hw, y + hh, z + hd, 0, 0);
		drawVertex(frontSide, x - hw, y - hh, z + hd, 0, 1);
		drawVertex(frontSide, x + hw, y - hh, z + hd, 1, 1);
		drawVertex(frontSide, x + hw, y + hh, z + hd, 1, 0);
		drawVertex(frontSide, x - hw, y + hh, z + hd, 0, 0);
		drawVertex(frontSide, x + hw, y - hh, z + hd, 1, 1);
	}

	if (topSide->shouldRender)
	{
		topSide->applyTexture(0, 0);
		glVertex3f(x - hw, y + hh, z - hd);
		glVertex3f(x - hw, y + hh, z + hd);
		glVertex3f(x + hw, y + hh, z + hd);
		glVertex3f(x + hw, y + hh, z - hd);
		glVertex3f(x - hw, y + hh, z - hd);
		glVertex3f(x + hw, y + hh, z + hd);
	}

	if (rightSide->shouldRender)
	{
		rightSide->applyTexture(0, 0);
		glVertex3f(x + hw, y + hh, z + hd);
		glVertex3f(x + hw, y - hh, z + hd);
		glVertex3f(x + hw, y - hh, z - hd);
		glVertex3f(x + hw, y + hh, z - hd);
		glVertex3f(x + hw, y + hh, z + hd);
		glVertex3f(x + hw, y - hh, z - hd);
	}

	if (backSide->shouldRender)
	{
		backSide->applyTexture(0, 0);
		glVertex3f(x + hw, y + hh, z - hd);
		glVertex3f(x + hw, y - hh, z - hd);
		glVertex3f(x - hw, y - hh, z - hd);
		glVertex3f(x - hw, y + hh, z - hd);
		glVertex3f(x + hw, y + hh, z - hd);
		glVertex3f(x - hw, y - hh, z - hd);
	}

	if (bottomSide->shouldRender)
	{
		bottomSide->applyTexture(0, 0);
		glVertex3f(x - hw, y - hh, z + hd);
		glVertex3f(x - hw, y - hh, z - hd);
		glVertex3f(x + hw, y - hh, z - hd);
		glVertex3f(x + hw, y - hh, z + hd);
		glVertex3f(x - hw, y - hh, z + hd);
		glVertex3f(x + hw, y - hh, z - hd);
	}

	
	if (leftSide->shouldRender)
	{
		leftSide->applyTexture(0, 0);
		glVertex3f(x - hw, y + hh, z - hd);
		glVertex3f(x - hw, y - hh, z - hd);
		glVertex3f(x - hw, y - hh, z + hd);
		glVertex3f(x - hw, y + hh, z + hd);
		glVertex3f(x - hw, y + hh, z - hd);
		glVertex3f(x - hw, y - hh, z + hd);
	}
}

void Block::drawVertex(BlockSide* side, GLfloat x, GLfloat y, GLfloat z, GLfloat texX, GLfloat texY)
{
	side->applyTexture(texX, texY);
	glVertex3f(x, y, z);
}

void Block::setPosition(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

// === SimpleBlockSide ===

SimpleBlockSide::SimpleBlockSide(Color4f color) : color(color)
{

}

void SimpleBlockSide::applyTexture(GLfloat texX, GLfloat texY)
{
	glColor4fv(color.color4fv);
}

// === TexturedBlockSide ===

TexturedBlockSide::TexturedBlockSide(GLfloat texX, GLfloat texY, GLfloat texW, GLfloat texH)
{
	this->x = texX;
	this->y = texY;
	this->w = texW;
	this->h = texH;
}

void TexturedBlockSide::applyTexture(GLfloat texX, GLfloat texY)
{
	glColor3f(1, 1, 1);
	glTexCoord2f(x + texX * w, y + texY * h);
}
