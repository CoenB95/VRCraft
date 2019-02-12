#ifndef CAMERA_H
#define CAMERA_H

#include "gameobject.h"
#include "vec.h"

class Camera : public GameObject
{
public:
	static const int CAMERA_TYPE_FIRST_PERSON = 0;
	static const int CAMERA_TYPE_THIRD_PERSON = 1;
	static const int CAMERA_TYPE_THIRD_PERSON_FRONT = 2;

	float thirdPersonDistance = 3.0f;
	int type = CAMERA_TYPE_FIRST_PERSON;
	float snappyness = 0.6f;

	void applyTransform();
	void setType(int type) { this->type = type; }
	void toggleType() { this->type = (this->type + 1) % 3; }
};

#endif // !CAMERA_H
