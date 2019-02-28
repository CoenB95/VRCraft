#pragma once

#include <VrLib/gl/Vertex.h>

using namespace glm;
using namespace std;

class Block;
class BlockContext;
class ChunkContext;
class Stack;

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
	vector<vrlib::gl::VertexP3N3T2> vertices;

public:
	Chunk(vec3 chunkSize, vec3 blockSize);

	void build(ChunkContext* chunkContext);
	Stack* destroyBlock(Block* block);
	void destroyStack(Stack* stack);
	BlockContext getAdjacentBlocks(ChunkContext* chunkContext, vec3 positionInChunk);
	Block* getBlock(vec3 positionInChunk);
	int getBlockIndex(vec3 positionInChunk);
	Block** getBlockPtr(vec3 positionInChunk);
	Stack* getNearbyStack(vec3 position, float maxDistance = 1.0f);
	bool isBlockTransparent(Block* block);
	void loadTextures();
	Stack* mergeStacks();
	void notifyBlockChanged(Block* newBlock);
	void notifyStackDropped(Stack* newStack);
	void notifyStackRemoved(Stack* oldStack);
	void randomTick(ChunkContext* chunkContext);
	void update(float elapsedSeconds) override;
};

class ChunkContext {
public:
	Chunk* top;
	Chunk* front;
	Chunk* right;
	Chunk* back;
	Chunk* left;
	Chunk* bottom;

	ChunkContext(Chunk* top, Chunk* front, Chunk* right, Chunk* back, Chunk* left, Chunk* bottom) :
		top(top), front(front), right(right), back(back), left(left), bottom(bottom) { };
};
