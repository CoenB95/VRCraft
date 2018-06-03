#ifndef CHUNK_H
#define CHUNK_H

#include <vector>

#include "block.h"
#include "gameobject.h"
#include "stack.h"

using namespace std;

class Chunk : public GameObject
{
private:
	float blockDrawSize = 1.0f;
	vector<Block*> blocks;
	vector<Block*> newBlocks;
	vector<Stack*> items;
	vector<Stack*> newItems;
	vector<Stack*> removedItems;
	bool blocksChanged = true;
	bool itemsChanged = false;
public:
	int width, height, depth;

	Chunk(int width, int height, int depth);

	Stack* destroyBlock(Block* block);
	void destroyStack(Stack* stack);
	void drawRaw();
	Block::BlockContext getAdjacentBlocks(Block* base);
	Block* getBlock(int index);
	Block* getBlock(int x, int y, int z);
	Block* getBlock(float x, float y, float z);
	int getBlockIndex(Block* block);
	int getBlockIndex(int x, int y, int z);
	Block** getBlockPtr(int x, int y, int z);
	Stack* getNearbyStack(Vec3f position, float maxDistance = 1.0f);
	bool isBlockTransparent(Block* block);
	void notifyBlockChanged(Block* newBlock);
	void notifyStackDropped(Stack* newStack);
	void notifyStackRemoved(Stack* oldStack);
	void update(float elapsedSeconds) override;
};

class ChunkDrawComponent : public DrawComponent
{
private:
	static GLuint terrainTextureId;

public:
	ChunkDrawComponent();
	void draw() override;
	static void loadTextures();
	void update(float elapsedSeconds) override {};
};

#endif // !CHUNK_H
