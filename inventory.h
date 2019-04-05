#pragma once

#include <vector>

#include "gameobject.h"

class Inventory : public GameObject {
private:
	vector<Stack*> content;
	int size;
};