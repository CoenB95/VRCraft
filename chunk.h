#pragma once

#include <mutex>
#include <VrLib/gl/Vertex.h>

using namespace glm;
using namespace std;

class Block;
class BlockContext;
class ChunkContext;
class Stack;
class World;

class Chunk : public GameObject {
private:
	vector<Block*> blocks;
	vec3 chunkSize;
	vec3 blockSize;
	vector<Block*> newBlocks;
	vector<Stack*> items;
	vector<Stack*> newItems;
	vector<Stack*> removedItems;
	bool blocksChanged = true;
	bool itemsChanged = false;

	void updateNewBlocks(ChunkContext* chunkContext);

protected:
	ChunkContext* context;

public:
	World* parentWorld;

	Chunk(vec3 chunkSize, vec3 blockSize);

	void build(vec3 offsetPosition) override;
	BlockContext* getAdjacentBlocks(ChunkContext* chunkContext, vec3 positionInChunk);
	Block* getBlock(vec3 positionInChunk);
	int getBlockIndex(vec3 positionInChunk);
	Block** getBlockPtr(vec3 positionInChunk);
	bool isBlockTransparent(Block* block);
	void loadTextures();
	void populateFromSeed(vec3 worldSize, int seed = 0);
	void randomTick();
	void setBlock(vec3 positionInChunk, Block* newBlock);
	void update(float elapsedSeconds) override;
	void updateAdjacentBlocks(ChunkContext* chunkContext, vec3 positionInChunk);
	void updateContext(ChunkContext* chunkContext);
};

class ChunkContext {
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

	Chunk* surroundings[3][3][3];

	Chunk** up =	&surroundings[X_CENTER][Y_TOP][Z_CENTER];
	Chunk** south =	&surroundings[X_CENTER][Y_CENTER][Z_FRONT];
	Chunk** east =	&surroundings[X_RIGHT][Y_CENTER][Z_CENTER];
	Chunk** north =	&surroundings[X_CENTER][Y_CENTER][Z_BACK];
	Chunk** west =	&surroundings[X_LEFT][Y_CENTER][Z_CENTER];
	Chunk** down =	&surroundings[X_CENTER][Y_BOTTOM][Z_CENTER];

	ChunkContext() { };
	ChunkContext(Chunk* up, Chunk* south, Chunk* east, Chunk* north, Chunk* west, Chunk* down) {
		*this->up = up;
		*this->south = south;
		*this->east = east;
		*this->north = north;
		*this->west = west;
		*this->down = down;
	};
};
