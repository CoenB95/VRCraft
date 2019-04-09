#pragma once

#include "gameobjectgroup.h"

class Player : public GameObjectGroup {
public:
	GameObject* feet;
	GameObject* primaryHand;
	GameObject* secondaryHand;

	Player();
};