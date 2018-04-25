#ifndef BLOCK_H
#define BLOCK_H

#include <GL/freeglut.h>

#include "color.h"

class BlockContext;

class BlockSide
{
public:
	bool shouldRender = true;
	virtual void applyTexture(GLfloat texX, GLfloat texY) = 0;
};

class SimpleBlockSide : public BlockSide
{
private:
	Color4f color;
public:
	SimpleBlockSide(Color4f color);
	void applyTexture(GLfloat texX, GLfloat texY) override;
};


class TexturedBlockSide : public BlockSide
{
private:
	GLfloat x, y, w, h;
public:
	TexturedBlockSide(GLfloat texX, GLfloat texY, GLfloat texW, GLfloat texH);
	void applyTexture(GLfloat texX, GLfloat texY) override;
};

class Block
{
private:
	GLfloat hw, hh, hd;
	void drawVertex(BlockSide* side, GLfloat x, GLfloat y, GLfloat z, GLfloat texX, GLfloat texY);
public:
	class BlockContext
	{
	public:
		Block* top;
		Block* front;
		Block* right;
		Block* back;
		Block* left;
		Block* bottom;
		BlockContext(Block* top, Block* front, Block* right, Block* back, Block* left, Block* bottom);
	};

public:
	static const GLfloat TILE_SIZE;

	BlockSide* backSide;
	BlockSide* bottomSide;
	BlockSide* frontSide;
	BlockSide* leftSide;
	BlockSide* rightSide;
	BlockSide* topSide;

	int x, y, z;
	bool isTransparent = false;

	Block();
	Block(float w, float h, float d);
	Block(BlockSide* top, BlockSide* front, BlockSide* right,
		BlockSide* back, BlockSide* left, BlockSide* bottom);
	Block(float w, float h, float d, BlockSide* top, BlockSide* front, BlockSide* right,
		BlockSide* back, BlockSide* left, BlockSide* bottom);

	void draw();
	void drawRaw();
	virtual Block* randomTick(Block::BlockContext& adjacentBlocks);
	void setColor(Color4f color);
	void setColors(Color4f front, Color4f top, Color4f right, Color4f back, Color4f bottom, Color4f left);
	void setPosition(int x, int y, int z);
};

inline void Block::setColor(Color4f color)
{
	setColors(color, color, color, color, color, color);
}

inline void Block::setColors(Color4f front, Color4f top, Color4f right, Color4f back, Color4f bottom, Color4f left)
{
	// Prevent memory leaks.
	delete backSide;
	delete bottomSide;
	delete frontSide;
	delete leftSide;
	delete rightSide;
	delete topSide;

	backSide = new SimpleBlockSide(back);
	bottomSide = new SimpleBlockSide(bottom);
	frontSide = new SimpleBlockSide(front);
	leftSide = new SimpleBlockSide(left);
	rightSide = new SimpleBlockSide(right);
	topSide = new SimpleBlockSide(top);
}

#endif // BLOCK_H
