#ifndef CAMERA_H
#define CAMERA_H

#include "vec.h"

class Camera
{
public:
	Vec3f pos;
	float rotX = 0;
	float rotY = 0;

	void applyTransform();
};

#endif // !CAMERA_H

