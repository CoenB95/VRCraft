#define SQ(a) ((a) * (a))

#include "vec.h"

Vec3f::Vec3f(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
Vec3f::Vec3f()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}
Vec3f::Vec3f(Vec3f &other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

float& Vec3f::operator [](int index)
{
	return values[index];
}

float Vec3f::distanceSquared(Vec3f &other)
{
	return SQ(other.x - this->x) + SQ(other.z - this->z);
}
