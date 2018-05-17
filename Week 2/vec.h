#pragma once

class Vec3f
{
public:
	union
	{
		struct
		{
			float x, y, z;
		};
		float values[3];
	};
	Vec3f();
	Vec3f(const Vec3f& other);
	Vec3f(float x, float y, float z);
	float& operator [](int);
	float distanceSquared(Vec3f& other);
	void set(int x, int y, int z);
	void set(float x, float y, float z);
	void set(Vec3f pos);
};

class Vec2f
{
public:
	union
	{
		struct
		{
			float x, y;
		};
		float v[2];
	};
	Vec2f();
	Vec2f(float x, float y);
	Vec2f(const Vec2f& other);
	float& operator [](int);
};