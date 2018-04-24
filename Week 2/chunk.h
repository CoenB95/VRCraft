#include <vector>

#include "block.h"

using namespace std;

class Chunk
{
private:
	int width, height, depth;
	vector<Block> blocks;
public:
	Chunk(int width, int height, int depth);
	void draw();
	void drawRaw();
};
