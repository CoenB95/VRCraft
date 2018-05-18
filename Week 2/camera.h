#ifndef CAMERA_H
#define CAMERA_H

#include "vec.h"

class Camera
{
public:
	static const int CAMERA_TYPE_FIRST_PERSON = 0;
	static const int CAMERA_TYPE_THIRD_PERSON = 1;
	static const int CAMERA_TYPE_THIRD_PERSON_FRONT = 2;

	Vec3f pos;
	float rotX = 0;
	float rotY = 0;
	float targetRotX = 0;
	float targetRotY = 0;
	float thirdPersonDistance = 3.0f;
	int type = CAMERA_TYPE_FIRST_PERSON;
	float snappyness = 0.6f;

	void applyTransform();
	void setType(int type) { this->type = type; }
	void toggleType() { this->type = (this->type + 1) % 3; }
	void update(float elasedSeconds);
};

#endif // !CAMERA_H

