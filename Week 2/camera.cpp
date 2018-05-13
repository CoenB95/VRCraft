#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES

#include <GL/freeglut.h>
#include <cmath>
#include <iostream>

#include "camera.h"

void Camera::applyTransform()
{
	// NOTE: The real z-axis is inverted. So apply it here.

	float rotRadX = (rotX) / 180.0f * M_PI;
	float rotRadY = (rotY - 90.0f) / 180.0f * M_PI;

	switch (type)
	{
	case CAMERA_TYPE_FIRST_PERSON:
		// Look from ourselves, to a point slightly in front.
		glRotatef(rotX, 1, 0, 0);
		glRotatef(rotY, 0, 1, 0);
		glTranslatef(-pos.x, -pos.y, pos.z);
		break;
	case CAMERA_TYPE_THIRD_PERSON:
		// Look from a point slightly behind, to ourselves.
		gluLookAt(
			pos.x - cosf(rotRadX) * cosf(rotRadY) * thirdPersonDistance,
			pos.y + sinf(rotRadX) * thirdPersonDistance,
			-pos.z - cosf(rotRadX) * sinf(rotRadY) * thirdPersonDistance,
			pos.x, pos.y, -pos.z, 0, 1, 0);
		break;
	case CAMERA_TYPE_THIRD_PERSON_FRONT:
		// Look from a point slightly in front, to ourselves.
		gluLookAt(
			pos.x + cosf(rotRadX) * cosf(rotRadY) * thirdPersonDistance,
			pos.y + sinf(rotRadX) * thirdPersonDistance,
			-pos.z + cosf(rotRadX) * sinf(rotRadY) * thirdPersonDistance,
			pos.x, pos.y, -pos.z, 0, 1, 0);
		break;
	}
}
