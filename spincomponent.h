#pragma once

#include "gameobjectcomponent.h"

class SpinComponent : public GameObjectComponent {
private:
	float degreesPerSec;
	float value;

public:
	SpinComponent(float degreesPerSec, string tag = "");

	void onUpdate(float elapsedSeconds);
};