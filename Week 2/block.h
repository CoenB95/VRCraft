#include "color.h"

class Block
{
private:
	Color4f backColor;
	Color4f bottomColor;
	Color4f frontColor;
	Color4f leftColor;
	Color4f rightColor;
	Color4f topColor;
public:
	float x, y;
	float width, height, depth;
	Block();
	Block(float w, float h, float d);
	void draw();
	void setColor(Color4f color);
	void setColors(Color4f front, Color4f top, Color4f right, Color4f back, Color4f bottom, Color4f left);
	void setPosition(float x, float y);
};

inline void Block::setColor(Color4f color)
{
	backColor = color;
	bottomColor = color;
	frontColor = color;
	leftColor = color;
	rightColor = color;
	topColor = color;
}

inline void Block::setColors(Color4f front, Color4f top, Color4f right, Color4f back, Color4f bottom, Color4f left)
{
	backColor = back;
	bottomColor = bottom;
	frontColor = front;
	leftColor = left;
	rightColor = right;
	topColor = top;
}
