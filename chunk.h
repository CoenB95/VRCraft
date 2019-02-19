#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <VrLib/Texture.h>

#include "block.h"
#include "gameobject.h"
#include "stack.h"

using namespace glm;
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
	vector<vrlib::gl::VertexP3N3T2> vertices;

public:
	int width, height, depth;

	Chunk(int width, int height, int depth);

	void build();
	Stack* destroyBlock(Block* block);
	void destroyStack(Stack* stack);
	//void drawRaw();
	Block::BlockContext getAdjacentBlocks(Block* base);
	Block* getBlock(int index);
	Block* getBlock(int x, int y, int z);
	Block* getBlock(float x, float y, float z);
	int getBlockIndex(Block* block);
	int getBlockIndex(int x, int y, int z);
	Block** getBlockPtr(int x, int y, int z);
	Stack* getNearbyStack(vec3 position, float maxDistance = 1.0f);
	bool isBlockTransparent(Block* block);
	void loadTextures();
	Stack* mergeStacks();
	void notifyBlockChanged(Block* newBlock);
	void notifyStackDropped(Stack* newStack);
	void notifyStackRemoved(Stack* oldStack);
	void randomUpdateBlock(Block* block);
	void update(float elapsedSeconds) override;
};

/*class ChunkDrawComponent : public DrawComponent
{
private:
	static vrlib::Texture* texture;

public:
	ChunkDrawComponent();
	void draw() override;
	static void loadTextures();
	void update(float elapsedSeconds) override {};
};*/

#endif // !CHUNK_H
