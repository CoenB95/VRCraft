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
const GLfloat Block::SCALE_BLOCK = 1.0f;
const GLfloat Block::SCALE_BLOCK_OVERLAY = 1.001f;
const GLfloat Block::SCALE_ITEM = 0.3f;

Block::Block(BlockSide* top, BlockSide* front, BlockSide* right,
	BlockSide* back, BlockSide* left, BlockSide* bottom, string typeName, GLfloat scale) :
	topSide(top),
	frontSide(front),
	rightSide(right),
	backSide(back),
	leftSide(left),
	bottomSide(bottom),
	typeName(typeName),
	pos(0, 0, 0)
{
	this->hw = scale / 2;
	this->hh = scale / 2;
	this->hd = scale / 2;
}

void Block::draw()
{
	glPushMatrix();

	glTranslatef(pos.x, pos.y, -pos.z);
	glBegin(GL_TRIANGLES);
	drawRaw(false);
	glEnd();

	glPopMatrix();
}

void Block::drawRaw(bool offset)
{
	if (isTransparent)
		return;

	GLfloat x = (offset ? this->pos.x : 0) * this->hw * 2;
	GLfloat y = (offset ? this->pos.y : 0) * this->hh * 2;
	GLfloat z = (offset ? this->pos.z : 0) * this->hd * 2;

	if (frontSide->shouldRender)
	{
		drawVertex(frontSide, x - hw, y + hh, z - hd, TOP_LEFT);
		drawVertex(frontSide, x - hw, y - hh, z - hd, BOTTOM_LEFT);
		drawVertex(frontSide, x + hw, y - hh, z - hd, BOTTOM_RIGHT);
		drawVertex(frontSide, x + hw, y + hh, z - hd, TOP_RIGHT);
		drawVertex(frontSide, x - hw, y + hh, z - hd, TOP_LEFT);
		drawVertex(frontSide, x + hw, y - hh, z - hd, BOTTOM_RIGHT);
	}

	if (topSide->shouldRender)
	{
		drawVertex(topSide, x - hw, y + hh, z + hd, TOP_LEFT);
		drawVertex(topSide, x - hw, y + hh, z - hd, BOTTOM_LEFT);
		drawVertex(topSide, x + hw, y + hh, z - hd, BOTTOM_RIGHT);
		drawVertex(topSide, x + hw, y + hh, z + hd, TOP_RIGHT);
		drawVertex(topSide, x - hw, y + hh, z + hd, TOP_LEFT);
		drawVertex(topSide, x + hw, y + hh, z - hd, BOTTOM_RIGHT);
	}

	if (rightSide->shouldRender)
	{
		drawVertex(rightSide, x + hw, y + hh, z - hd, TOP_LEFT);
		drawVertex(rightSide, x + hw, y - hh, z - hd, BOTTOM_LEFT);
		drawVertex(rightSide, x + hw, y - hh, z + hd, BOTTOM_RIGHT);
		drawVertex(rightSide, x + hw, y + hh, z + hd, TOP_RIGHT);
		drawVertex(rightSide, x + hw, y + hh, z - hd, TOP_LEFT);
		drawVertex(rightSide, x + hw, y - hh, z + hd, BOTTOM_RIGHT);
	}

	if (backSide->shouldRender)
	{
		drawVertex(backSide, x + hw, y + hh, z + hd, TOP_LEFT);
		drawVertex(backSide, x + hw, y - hh, z + hd, BOTTOM_LEFT);
		drawVertex(backSide, x - hw, y - hh, z + hd, BOTTOM_RIGHT);
		drawVertex(backSide, x - hw, y + hh, z + hd, TOP_RIGHT);
		drawVertex(backSide, x + hw, y + hh, z + hd, TOP_LEFT);
		drawVertex(backSide, x - hw, y - hh, z + hd, BOTTOM_RIGHT);
	}

	if (bottomSide->shouldRender)
	{
		drawVertex(bottomSide, x - hw, y - hh, z - hd, TOP_LEFT);
		drawVertex(bottomSide, x - hw, y - hh, z + hd, BOTTOM_LEFT);
		drawVertex(bottomSide, x + hw, y - hh, z + hd, BOTTOM_RIGHT);
		drawVertex(bottomSide, x + hw, y - hh, z - hd, TOP_RIGHT);
		drawVertex(bottomSide, x - hw, y - hh, z - hd, TOP_LEFT);
		drawVertex(bottomSide, x + hw, y - hh, z + hd, BOTTOM_RIGHT);
	}

	
	if (leftSide->shouldRender)
	{
		drawVertex(leftSide, x - hw, y + hh, z + hd, TOP_LEFT);
		drawVertex(leftSide, x - hw, y - hh, z + hd, BOTTOM_LEFT);
		drawVertex(leftSide, x - hw, y - hh, z - hd, BOTTOM_RIGHT);
		drawVertex(leftSide, x - hw, y + hh, z - hd, TOP_RIGHT);
		drawVertex(leftSide, x - hw, y + hh, z + hd, TOP_LEFT);
		drawVertex(leftSide, x - hw, y - hh, z - hd, BOTTOM_RIGHT);
	}
}

void Block::drawVertex(BlockSide* side, GLfloat x, GLfloat y, GLfloat z, GLfloat texX, GLfloat texY)
{
	side->applyTexture(texX, texY);

	// The real z-axis is inverted. Apply it here.
	glVertex3f(x, y, -z);
}

string Block::getPositionString() const
{
	stringstream ss;
	ss << "x=" << pos.x << ", y=" << pos.y << ", z=" << pos.z;
	return ss.str();
}

Block* Block::randomTick(Block::BlockContext& adjacentBlocks)
{
	return nullptr;
}

string Block::toString() const
{
	stringstream ss;
	ss << (isTransparent ? "Air" : typeName) << "{" << getPositionString() << "}";
	return ss.str();
}

Block::BlockContext::BlockContext(Block* top, Block* front, Block* right, Block* back, Block* left, Block* bottom) :
	top(top), front(front), right(right), back(back), left(left), bottom(bottom)
{

}

Block* Block::BlockContext::operator [](int index)
{
	switch (index)
	{
	case TOP_SIDE: return top;
	case FRONT_SIDE: return front;
	case RIGHT_SIDE: return right;
	case BACK_SIDE: return back;
	case LEFT_SIDE: return left;
	case BOTTOM_SIDE: return bottom;
	default: return nullptr;
	}
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

TexturedBlockSide::TexturedBlockSide(GLint texX, GLint texY, GLfloat texW, GLfloat texH, Color4f color)
	: color(color)
{
	this->x = (GLfloat)texX * Block::TILE_SIZE;
	this->y = (GLfloat)texY * Block::TILE_SIZE;
	this->w = texW * Block::TILE_SIZE;
	this->h = texH * Block::TILE_SIZE;
}

void TexturedBlockSide::applyTexture(GLfloat texX, GLfloat texY)
{
	glColor4fv(color.color4fv);
	glTexCoord2f(x + texX * w, y + texY * h);
}

SelectionBlock::SelectionBlock(float breakage) : Block(
	new TexturedBlockSide((GLint)(breakage * 10) - 1, 15, 1, 1, Color4f(1, 1, 1, (GLint)(breakage * 10) < 1 ? 0 : 1)),
	new TexturedBlockSide((GLint)(breakage * 10) - 1, 15, 1, 1, Color4f(1, 1, 1, (GLint)(breakage * 10) < 1 ? 0 : 1)),
	new TexturedBlockSide((GLint)(breakage * 10) - 1, 15, 1, 1, Color4f(1, 1, 1, (GLint)(breakage * 10) < 1 ? 0 : 1)),
	new TexturedBlockSide((GLint)(breakage * 10) - 1, 15, 1, 1, Color4f(1, 1, 1, (GLint)(breakage * 10) < 1 ? 0 : 1)),
	new TexturedBlockSide((GLint)(breakage * 10) - 1, 15, 1, 1, Color4f(1, 1, 1, (GLint)(breakage * 10) < 1 ? 0 : 1)),
	new TexturedBlockSide((GLint)(breakage * 10) - 1, 15, 1, 1, Color4f(1, 1, 1, (GLint)(breakage * 10) < 1 ? 0 : 1)),
	"Breakage ",
	SCALE_BLOCK_OVERLAY)
{

}
