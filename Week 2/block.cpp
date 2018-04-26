#define BOTTOM_LEFT		0, 1
#define BOTTOM_RIGHT	1, 1
#define TOP_LEFT		0, 0
#define TOP_RIGHT		1, 0

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES

#include <GL/freeglut.h>
#include <cmath>
#include <string>
#include <sstream>

#include "block.h"

using namespace std;

const GLfloat Block::TILE_SIZE = 1.0f / 256.0f * 16.0f;

Block::Block() : Block(1, 1, 1)
{
	
}

Block::Block(float w, float h, float d) : Block(w, h, d,
	new TexturedBlockSide(0, 0, 1, 1),
	new TexturedBlockSide(3, 0, 1, 1),
	new TexturedBlockSide(3, 0, 1, 1),
	new TexturedBlockSide(3, 0, 1, 1),
	new TexturedBlockSide(3, 0, 1, 1),
	new TexturedBlockSide(2, 0, 1, 1))
{

}

Block::Block(BlockSide* top, BlockSide* front, BlockSide* right,
	BlockSide* back, BlockSide* left, BlockSide* bottom) : Block(1, 1, 1, top, front, right, back, left, bottom)
{

}

Block::Block(float w, float h, float d, BlockSide* top, BlockSide* front, BlockSide* right,
	BlockSide* back, BlockSide* left, BlockSide* bottom) :
	topSide(top),
	frontSide(front),
	rightSide(right),
	backSide(back),
	leftSide(left),
	bottomSide(bottom)
{
	this->x = 0;
	this->y = 0;
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

	GLfloat x = this->x * this->hw * 2;
	GLfloat y = this->y * this->hh * 2;
	GLfloat z = this->z * this->hd * 2;

	if (frontSide->shouldRender)
	{
		drawVertex(frontSide, x - hw, y + hh, z + hd, TOP_LEFT);
		drawVertex(frontSide, x - hw, y - hh, z + hd, BOTTOM_LEFT);
		drawVertex(frontSide, x + hw, y - hh, z + hd, BOTTOM_RIGHT);
		drawVertex(frontSide, x + hw, y + hh, z + hd, TOP_RIGHT);
		drawVertex(frontSide, x - hw, y + hh, z + hd, TOP_LEFT);
		drawVertex(frontSide, x + hw, y - hh, z + hd, BOTTOM_RIGHT);
	}

	if (topSide->shouldRender)
	{
		drawVertex(topSide, x - hw, y + hh, z - hd, TOP_LEFT);
		drawVertex(topSide, x - hw, y + hh, z + hd, BOTTOM_LEFT);
		drawVertex(topSide, x + hw, y + hh, z + hd, BOTTOM_RIGHT);
		drawVertex(topSide, x + hw, y + hh, z - hd, TOP_RIGHT);
		drawVertex(topSide, x - hw, y + hh, z - hd, TOP_LEFT);
		drawVertex(topSide, x + hw, y + hh, z + hd, BOTTOM_RIGHT);
	}

	if (rightSide->shouldRender)
	{
		drawVertex(rightSide, x + hw, y + hh, z + hd, TOP_LEFT);
		drawVertex(rightSide, x + hw, y - hh, z + hd, BOTTOM_LEFT);
		drawVertex(rightSide, x + hw, y - hh, z - hd, BOTTOM_RIGHT);
		drawVertex(rightSide, x + hw, y + hh, z - hd, TOP_RIGHT);
		drawVertex(rightSide, x + hw, y + hh, z + hd, TOP_LEFT);
		drawVertex(rightSide, x + hw, y - hh, z - hd, BOTTOM_RIGHT);
	}

	if (backSide->shouldRender)
	{
		drawVertex(backSide, x + hw, y + hh, z - hd, TOP_LEFT);
		drawVertex(backSide, x + hw, y - hh, z - hd, BOTTOM_LEFT);
		drawVertex(backSide, x - hw, y - hh, z - hd, BOTTOM_RIGHT);
		drawVertex(backSide, x - hw, y + hh, z - hd, TOP_RIGHT);
		drawVertex(backSide, x + hw, y + hh, z - hd, TOP_LEFT);
		drawVertex(backSide, x - hw, y - hh, z - hd, BOTTOM_RIGHT);
	}

	if (bottomSide->shouldRender)
	{
		drawVertex(bottomSide, x - hw, y - hh, z + hd, TOP_LEFT);
		drawVertex(bottomSide, x - hw, y - hh, z - hd, BOTTOM_LEFT);
		drawVertex(bottomSide, x + hw, y - hh, z - hd, BOTTOM_RIGHT);
		drawVertex(bottomSide, x + hw, y - hh, z + hd, TOP_RIGHT);
		drawVertex(bottomSide, x - hw, y - hh, z + hd, TOP_LEFT);
		drawVertex(bottomSide, x + hw, y - hh, z - hd, BOTTOM_RIGHT);
	}

	
	if (leftSide->shouldRender)
	{
		drawVertex(leftSide, x - hw, y + hh, z - hd, TOP_LEFT);
		drawVertex(leftSide, x - hw, y - hh, z - hd, BOTTOM_LEFT);
		drawVertex(leftSide, x - hw, y - hh, z + hd, BOTTOM_RIGHT);
		drawVertex(leftSide, x - hw, y + hh, z + hd, TOP_RIGHT);
		drawVertex(leftSide, x - hw, y + hh, z - hd, TOP_LEFT);
		drawVertex(leftSide, x - hw, y - hh, z + hd, BOTTOM_RIGHT);
	}
}

void Block::drawVertex(BlockSide* side, GLfloat x, GLfloat y, GLfloat z, GLfloat texX, GLfloat texY)
{
	side->applyTexture(texX, texY);
	glVertex3f(x, y, z * -1);
}

string Block::getPositionString() const
{
	stringstream ss;
	ss << "x=" << x << ", y=" << y << ", z=" << z;
	return ss.str();
}

Block* Block::randomTick(Block::BlockContext& adjacentBlocks)
{
	return nullptr;
}

void Block::setPosition(int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

string Block::toString() const
{
	stringstream ss;
	ss << (isTransparent ? "Air" : "Block") << "{" << getPositionString() << "}";
	return ss.str();
}

Block::BlockContext::BlockContext(Block* top, Block* front, Block* right, Block* back, Block* left, Block* bottom) :
	top(top), front(front), right(right), back(back), left(left), bottom(bottom)
{

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
	this->x = texX * Block::TILE_SIZE;
	this->y = texY * Block::TILE_SIZE;
	this->w = texW * Block::TILE_SIZE;
	this->h = texH * Block::TILE_SIZE;
}

void TexturedBlockSide::applyTexture(GLfloat texX, GLfloat texY)
{
	glColor3f(1, 1, 1);
	glTexCoord2f(x + texX * w, y + texY * h);
}
