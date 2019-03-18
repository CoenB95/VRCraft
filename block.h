#pragma once

using namespace std;

#include <GL/glew.h>
#include <string>

#include "gameobject.h"
#include "chunk.h"

using namespace std;

class BlockContext;
class Chunk;

class Block : public GameObject {
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

	Block();

	string getPositionString() const;
	vec3 globalPosition() override;
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
	static const int Z_BACK = 0;
	static const int Z_CENTER = 1;
	static const int Z_FRONT = 2;

	Block* surroundings[3][3][3];

	Block** up =	&surroundings[X_CENTER][Y_TOP][Z_CENTER];
	Block** south =	&surroundings[X_CENTER][Y_CENTER][Z_FRONT];
	Block** east =	&surroundings[X_RIGHT][Y_CENTER][Z_CENTER];
	Block** north =	&surroundings[X_CENTER][Y_CENTER][Z_BACK];
	Block** west =	&surroundings[X_LEFT][Y_CENTER][Z_CENTER];
	Block** down =	&surroundings[X_CENTER][Y_BOTTOM][Z_CENTER];

	BlockContext() { };
	BlockContext(Block* up, Block* south, Block* east, Block* north, Block* west, Block* down) {
		*this->up = up;
		*this->south = south;
		*this->east = east;
		*this->north = north;
		*this->west = west;
		*this->down = down;
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
	CubeBlock(int all, bool transparent = false)
		: CubeBlock(all, all, all, all, all, all, transparent) {};
	CubeBlock(int top, int front, int right, int back, int left, int bottom, bool transparent = false);
	void build(vec3 offsetPosition) override;
};
