#pragma once

class World;
class Stack;

class Mob : public GameObject {
private:
	vector<Stack*> inventory;
	int inventorySize;
	World* world;

public:
	Mob(World* world);
};

class Steve : public Mob
{
public:
	Steve(World* world);
};
