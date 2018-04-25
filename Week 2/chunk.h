#ifndef CHUNK_H
#define CHUNK_H

#include <vector>

#include "block.h"

using namespace std;

class Chunk
{
private:
	int width, height, depth;
	float blockSize = 1.0f;
	float blockDrawSize = 1.0f;
	vector<Block*> blocks;
	vector<Block*> newBlocks;
	bool blocksChanged = true;
public:
	Chunk(int width, int height, int depth);
	void draw();
	void drawRaw();
	Block* getBlock(int x, int y, int z);
	Block** getBlockPtr(int x, int y, int z);
	void notifyBlockChanged(Block* newBlock);
	void update();
};

#endif // !CHUNK_H
