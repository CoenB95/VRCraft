#ifndef CHUNK_H
#define CHUNK_H

#include <vector>

#include "block.h"

using namespace std;

class Chunk
{
private:
	float blockDrawSize = 1.0f;
	vector<Block*> blocks;
	vector<Block*> newBlocks;
	bool blocksChanged = true;
public:
	int width, height, depth;

	Chunk(int width, int height, int depth);

	void draw();
	void drawRaw();
	Block::BlockContext getAdjacentBlocks(Block* base);
	Block* getBlock(int index);
	Block* getBlock(int x, int y, int z);
	Block* getBlock(float x, float y, float z);
	int getBlockIndex(Block* block);
	int getBlockIndex(int x, int y, int z);
	Block** getBlockPtr(int x, int y, int z);
	bool isBlockTransparent(Block* block);
	void notifyBlockChanged(Block* newBlock);
	void update();
};

#endif // !CHUNK_H
