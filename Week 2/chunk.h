#include <vector>

#include "block.h"

using namespace std;

class Chunk
{
private:
	int width, height, depth;
	float blockSize = 1.0f;
	float blockDrawSize = 1.0f;
	vector<Block> blocks;
	bool blocksChanged = true;
public:
	Chunk(int width, int height, int depth);
	void draw();
	void drawRaw();
	Block* getBlock(int x, int y, int z);
	void update();
};
