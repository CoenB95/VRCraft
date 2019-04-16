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
	vec3 blockSize;

public:
	static const int TILES_HEIGHT_COUNT;
	static const int TILES_WIDTH_COUNT;
	static const int TILE_COUNT;
	static const GLfloat TILE_SIZE;
	static const GLfloat SCALE_BLOCK;
	static const GLfloat SCALE_BLOCK_OVERLAY;
	static const GLfloat SCALE_ITEM;

	//Whether this block should be considered see-through (render adjacent block-sides).
	bool isTransparent = false;

	//Pointer to the block that should replace this one.
	Block* newBlock = nullptr;

	Block(vec3 blockSize = vec3(1, 1, 1));
	Block(const Block* original);
	virtual Block* clone() const { return(new Block(this)); };

	vec3 getBlockSize() { return blockSize; };
	string getPositionString() const;
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

	Block* surroundings[3][3][3] = {};

	Block** up =	 &surroundings[X_CENTER][Y_TOP][Z_CENTER];
	Block** south =	 &surroundings[X_CENTER][Y_CENTER][Z_FRONT];
	Block** east =	 &surroundings[X_RIGHT][Y_CENTER][Z_CENTER];
	Block** north =	 &surroundings[X_CENTER][Y_CENTER][Z_BACK];
	Block** west =	 &surroundings[X_LEFT][Y_CENTER][Z_CENTER];
	Block** down =	 &surroundings[X_CENTER][Y_BOTTOM][Z_CENTER];
	Block** center = &surroundings[X_CENTER][Y_CENTER][Z_CENTER];

	BlockContext() { };
	bool anyAdjacent();
};

class CubeBlock : public Block {
private:
	int backTextureIndex = -1;
	int bottomTextureIndex = -1;
	int frontTextureIndex = -1;
	int leftTextureIndex = -1;
	int rightTextureIndex = -1;
	int topTextureIndex = -1;

public:
	CubeBlock(int all, vec3 blockSize = vec3(1, 1, 1), bool transparent = false)
		: CubeBlock(all, all, all, all, all, all, blockSize, transparent) {};
	CubeBlock(int top, int front, int right, int back, int left, int bottom, vec3 blockSize = vec3(1, 1, 1), bool transparent = false);
	CubeBlock(const CubeBlock* original);
	virtual Block* clone() const override { return(new CubeBlock(this)); };

	void build(vec3 offsetPosition) override;
	void setTextureIndexes(int all) { setTextureIndexes(all, all, all, all, all, all); }
	void setTextureIndexes(int top, int front, int right, int back, int left, int bottom);
};
