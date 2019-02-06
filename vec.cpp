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
Vec3f::Vec3f(const Vec3f& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

void Vec3f::operator +=(Vec3f other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
}

Vec3f Vec3f::operator *(float other)
{
	Vec3f result(x * other, y * other, z * other);
	return result;
}

float& Vec3f::operator [](int index)
{
	return values[index];
}

float Vec3f::distanceSquared(Vec3f &other)
{
	return SQ(other.x - this->x) + SQ(other.z - this->z);
}

void Vec3f::set(int x, int y, int z)
{
	set((float)x, (float)y, (float)z);
}

void Vec3f::set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vec3f::set(Vec3f pos)
{
	set(pos.x, pos.y, pos.z);
}

Vec2f::Vec2f(float x, float y)
{
	this->x = x;
	this->y = y;
}
Vec2f::Vec2f()
{
	this->x = 0;
	this->y = 0;
}
Vec2f::Vec2f(const Vec2f& other)
{
	this->x = other.x;
	this->y = other.y;
}

float& Vec2f::operator [](int index)
{
	return v[index];
}
