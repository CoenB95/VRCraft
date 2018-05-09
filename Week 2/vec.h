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
	Vec3f(Vec3f &other);
	Vec3f(float x, float y, float z);
	float& operator [](int);
	float distanceSquared(Vec3f& other);
	void set(float x, float y, float z);
};
