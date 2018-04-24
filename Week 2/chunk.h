#include <vector>

#include "block.h"

using namespace std;

class Chunk
{
private:
	int width, depth;
	vector<Block> blocks;
public:
	Chunk(int width, int depth);
	void draw();
	void drawRaw();
};
