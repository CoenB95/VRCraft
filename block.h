#pragma once

using namespace std;

#include <GL/glew.h>
#include <string>

#include "gameobject.h"

using namespace std;

class BlockContext;
class Chunk;

class Block : public GameObject {
private:
	string typeName;

protected:
	BlockContext* context = nullptr;

public:
	static const int TILES_HEIGHT_COUNT;
	static const int TILES_WIDTH_COUNT;
	static const int TILE_COUNT;
	static const GLfloat TILE_SIZE;
	static const GLfloat SCALE_BLOCK;
	static const GLfloat SCALE_BLOCK_OVERLAY;
	static const GLfloat SCALE_ITEM;

	Chunk* parentChunk = nullptr;

	//Whether this block should be considered see-through (render adjacent block-sides).
	bool isTransparent = false;

	//Pointer to the block that should replace this one.
	Block* newBlock = nullptr;

	Block(string typeName = "Unknown");

	string getPositionString() const;
	inline string getTypeName() { return typeName; };
	inline bool needsContext() { return context == nullptr; };
	virtual void randomTick() {};
	virtual string toString() const;
	void updateContext(BlockContext* blockContext);
};

class BlockContext {
public:
	static const int X_LEFT = 0;
	static const int X_CENTER = 1;
	static const int X_RIGHT = 2;
	static const int Y_BOTTOM = 0;
	static const int Y_CENTER = 1;
	static const int Y_TOP = 2;
	static const int Z_FRONT = 0;
	static const int Z_CENTER = 1;
	static const int Z_BACK = 2;

	Block* surroundings[3][3][3];

	Block* top = nullptr;
	Block* front = nullptr;
	Block* right = nullptr;
	Block* back = nullptr;
	Block* left = nullptr;
	Block* bottom = nullptr;

	BlockContext() { };
	BlockContext(Block* top, Block* front, Block* right, Block* back, Block* left, Block* bottom) {
		surroundings[X_CENTER][Y_TOP][Z_CENTER] = this->top = top;
		surroundings[X_CENTER][Y_CENTER][Z_FRONT] = this->front = front;
		surroundings[X_RIGHT][Y_CENTER][Z_CENTER] = this->right = right;
		surroundings[X_CENTER][Y_CENTER][Z_BACK] = this->back = back;
		surroundings[X_LEFT][Y_CENTER][Z_CENTER] = this->left = left;
		surroundings[X_CENTER][Y_BOTTOM][Z_CENTER] = this->bottom = bottom;
	};
	void updateSides() {
		top = surroundings[X_CENTER][Y_TOP][Z_CENTER];
		front = surroundings[X_CENTER][Y_CENTER][Z_FRONT];
		right = surroundings[X_RIGHT][Y_CENTER][Z_CENTER];
		back = surroundings[X_CENTER][Y_CENTER][Z_BACK];
		left = surroundings[X_LEFT][Y_CENTER][Z_CENTER];
		bottom = surroundings[X_CENTER][Y_BOTTOM][Z_CENTER];
	};
};

class CubeBlock : public Block {
private:
	int backTextureIndex;
	int bottomTextureIndex;
	int frontTextureIndex;
	int leftTextureIndex;
	int rightTextureIndex;
	int topTextureIndex;

public:
	CubeBlock(int all, string typeName, bool transparent = false)
		: CubeBlock(all, all, all, all, all, all, typeName, transparent) {};
	CubeBlock(int top, int front, int right, int back, int left, int bottom, string typeName, bool transparent = false);
	void build() override;
};

/*class SelectionBlock : public Block {
private:
	float resistance = 10.0f;
	float curTime = 0.0f;
public:
	SelectionBlock(float breakage);
};*/

class AirBlock : public CubeBlock {
public:
	AirBlock() : CubeBlock(-1, "Air", true) { };
};
