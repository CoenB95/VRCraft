#include <GL/freeglut.h>

#include "camera.h"

void Camera::applyTransform()
{
	glRotatef(rotX, 1, 0, 0);
	glRotatef(rotY, 0, 1, 0);
	// The real z-axis is inverted. Apply it here.
	glTranslatef(-pos.x, -pos.y, pos.z);
}
