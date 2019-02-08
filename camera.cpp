#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES

#include <GL/glew.h>
#include <cmath>
#include <iostream>

#include "camera.h"

void Camera::applyTransform()
{
	// NOTE: The real z-axis is inverted. So apply it here.

	float rotRadX = (rotateX) / 180.0f * M_PI;
	float rotRadY = (rotateY - 90.0f) / 180.0f * M_PI;

	switch (type)
	{
	case CAMERA_TYPE_FIRST_PERSON:
		// Look from ourselves, to a point slightly in front.
		glRotatef(rotateX, 1, 0, 0);
		glRotatef(rotateY, 0, 1, 0);
		glTranslatef(-position.x, -position.y, position.z);
		break;
	case CAMERA_TYPE_THIRD_PERSON:
		// Look from a point slightly behind, to ourselves.
		gluLookAt(
			position.x - cosf(rotRadX) * cosf(rotRadY) * thirdPersonDistance,
			position.y + sinf(rotRadX) * thirdPersonDistance,
			-position.z - cosf(rotRadX) * sinf(rotRadY) * thirdPersonDistance,
			position.x, position.y, -position.z, 0, 1, 0);
		break;
	case CAMERA_TYPE_THIRD_PERSON_FRONT:
		// Look from a point slightly in front, to ourselves.
		gluLookAt(
			position.x + cosf(rotRadX) * cosf(rotRadY) * thirdPersonDistance,
			position.y - sinf(rotRadX) * thirdPersonDistance,
			-position.z + cosf(rotRadX) * sinf(rotRadY) * thirdPersonDistance,
			position.x, position.y, -position.z, 0, 1, 0);
		break;
	}
}

/*void Camera::update(float elapsedSeconds)
{
	//rotateX = (snappyness * rotateX) + ((1.0f - snappyness) * targetRotX);
	//rotateY = (snappyness * rotateY) + ((1.0f - snappyness) * targetRotY);
}*/
