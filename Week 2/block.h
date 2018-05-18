#ifndef BLOCK_H
#define BLOCK_H

#include <GL/freeglut.h>
#include <string>

#include "color.h"
#include "vec.h"

using namespace std;

class BlockContext;

class BlockSide
{
public:
	bool shouldRender = true;
	virtual void applyTexture(GLfloat texX, GLfloat texY, bool hovered) = 0;
};

class SimpleBlockSide : public BlockSide
{
private:
	Color4f color;
public:
	SimpleBlockSide(Color4f color);
	void applyTexture(GLfloat texX, GLfloat texY, bool hovered) override;
};


class TexturedBlockSide : public BlockSide
{
private:
	GLfloat x, y, w, h;
public:
	TexturedBlockSide(GLint texX, GLint texY, GLfloat texW = 1.0f, GLfloat texH = 1.0f);
	void applyTexture(GLfloat texX, GLfloat texY, bool hovered) override;
};

class Block
{
private:
	GLfloat hw, hh, hd;
	void drawVertex(BlockSide* side, GLfloat x, GLfloat y, GLfloat z, GLfloat texX, GLfloat texY);
	string typeName;

public:
	class BlockContext
	{
	public:
		static const char TOP_SIDE = 0;
		static const char FRONT_SIDE = 1;
		static const char RIGHT_SIDE = 2;
		static const char BACK_SIDE = 3;
		static const char LEFT_SIDE = 4;
		static const char BOTTOM_SIDE = 5;

		Block* top;
		Block* front;
		Block* right;
		Block* back;
		Block* left;
		Block* bottom;
		BlockContext(Block* top, Block* front, Block* right, Block* back, Block* left, Block* bottom);
		Block* operator [](int index);
	};

public:
	static const GLfloat TILE_SIZE;

	BlockSide* backSide;
	BlockSide* bottomSide;
	BlockSide* frontSide;
	BlockSide* leftSide;
	BlockSide* rightSide;
	BlockSide* topSide;

	Vec3f pos;
	bool isTransparent = false;
	bool mark = false;

	Block(BlockSide* top, BlockSide* front, BlockSide* right,
		BlockSide* back, BlockSide* left, BlockSide* bottom, string typeName = "Unknown");
	Block(float w, float h, float d, BlockSide* top, BlockSide* front, BlockSide* right,
		BlockSide* back, BlockSide* left, BlockSide* bottom, string typeName = "Unknown");

	void draw();
	void drawRaw();
	string getPositionString() const;
	virtual Block* randomTick(Block::BlockContext& adjacentBlocks);
	void setColor(Color4f color);
	void setColors(Color4f front, Color4f top, Color4f right, Color4f back, Color4f bottom, Color4f left);
	virtual string toString() const;
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
