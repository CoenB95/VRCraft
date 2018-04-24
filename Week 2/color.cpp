#include "color.h"

Color4f::Color4f(float r, float g, float b)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 1;
}

Color4f::Color4f(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

const Color4f Color4f::BLUE(0, 0, 1);
const Color4f Color4f::CYAN(0, 1, 1);
const Color4f Color4f::GREEN(0, 1, 0);
const Color4f Color4f::MAGENTA(1, 0, 1);
const Color4f Color4f::RED(1, 0, 0);
const Color4f Color4f::YELLOW(1, 1, 0);
