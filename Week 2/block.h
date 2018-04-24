#ifndef BLOCK_H
#define BLOCK_H

#include "color.h"

class BlockSide
{
private:
	Color4f color;
public:
	bool shouldRender = true;

	BlockSide(Color4f color);

	void applyTexture();
	inline Color4f getColor()
	{
		return color;
	};
};

class Block
{
public:
	BlockSide backSide;
	BlockSide bottomSide;
	BlockSide frontSide;
	BlockSide leftSide;
	BlockSide rightSide;
	BlockSide topSide;

	float x, y, z;
	float width, height, depth;
	bool isTransparent = false;

	Block();
	Block(float w, float h, float d);

	void draw();
	void drawRaw();
	void setColor(Color4f color);
	void setColors(Color4f front, Color4f top, Color4f right, Color4f back, Color4f bottom, Color4f left);
	void setPosition(float x, float y, float z);
};

inline void Block::setColor(Color4f color)
{
	backSide = BlockSide(color);
	bottomSide = BlockSide(color);
	frontSide = BlockSide(color);
	leftSide = BlockSide(color);
	rightSide = BlockSide(color);
	topSide = BlockSide(color);
}

inline void Block::setColors(Color4f front, Color4f top, Color4f right, Color4f back, Color4f bottom, Color4f left)
{
	backSide = BlockSide(back);
	bottomSide = BlockSide(bottom);
	frontSide = BlockSide(front);
	leftSide = BlockSide(left);
	rightSide = BlockSide(right);
	topSide = BlockSide(top);
}

#endif // BLOCK_H
