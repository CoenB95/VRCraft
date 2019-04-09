#pragma once

#include <glm/glm.hpp>

#include "gameobjectgroup.h"

using namespace glm;

class Player : public GameObjectGroup {
public:
	GameObject* feet;
	GameObject* primaryHand;
	GameObject* secondaryHand;
	vec3 size = vec3(1, 2, 1);

	Player();
};