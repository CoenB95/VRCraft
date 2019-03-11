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

	void updateNewBlocks();

protected:
	ChunkContext* context;

public:
	World* parentWorld;

	Chunk(vec3 chunkSize, vec3 blockSize);

	void build();
	BlockContext* getAdjacentBlocks(ChunkContext* chunkContext, vec3 positionInChunk);
	Block* getBlock(vec3 positionInChunk);
	int getBlockIndex(vec3 positionInChunk);
	Block** getBlockPtr(vec3 positionInChunk);
	bool isBlockTransparent(Block* block);
	void loadTextures();
	void randomTick();
	void setBlock(vec3 positionInChunk, Block* newBlock);
	void update(float elapsedSeconds) override;
	void updateContext(ChunkContext* chunkContext);
};

class ChunkContext {
public:
	Chunk* top = nullptr;
	Chunk* front = nullptr;
	Chunk* right = nullptr;
	Chunk* back = nullptr;
	Chunk* left = nullptr;
	Chunk* bottom = nullptr;

	ChunkContext() { };
	ChunkContext(Chunk* top, Chunk* front, Chunk* right, Chunk* back, Chunk* left, Chunk* bottom) :
		top(top), front(front), right(right), back(back), left(left), bottom(bottom) { };
};
