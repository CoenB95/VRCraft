#ifndef BLOCK_H
#define BLOCK_H

#include <GL/glew.h>
#include <string>
#include <vector>
#include <VrLib/gl/Vertex.h>

#include "color.h"
#include "gameobject.h"
#include "gameobjectcomponent.h"

using namespace std;

class BlockContext;

class Block : public GameObject
{
private:
	string typeName;

public:
	class BlockContext
	{
	public:
		static const char TOP_SIDE = 0;
		static const char FRONT_SIDE = 1;
		static const char RIGHT_SIDE = 2;
		static const char BACK_SIDE = 3;
		static const char LEFT_SIDE = 4;
		static const char BOTTOM_SIDE = 5;

		Block* top;
		Block* front;
		Block* right;
		Block* back;
		Block* left;
		Block* bottom;

		BlockContext(Block* top, Block* front, Block* right, Block* back, Block* left, Block* bottom);
		Block* operator [](int index);
	};

public:
	static const int TILES_HEIGHT_COUNT;
	static const int TILES_WIDTH_COUNT;
	static const int TILE_COUNT;
	static const GLfloat TILE_SIZE;
	static const GLfloat SCALE_BLOCK;
	static const GLfloat SCALE_BLOCK_OVERLAY;
	static const GLfloat SCALE_ITEM;

	bool isTransparent = false;
	bool shouldRebuild = true;
	std::vector<vrlib::gl::VertexP3N3T2> vertices;

	Block(string typeName = "Unknown");

	inline virtual void build(BlockContext& context) {};
	string getPositionString() const;
	inline string getTypeName() { return typeName; };
	virtual Block* randomTick(Block::BlockContext& context);
	virtual string toString() const;
};

class CubeBlock : public Block
{
private:
	int backTextureIndex;
	int bottomTextureIndex;
	int frontTextureIndex;
	int leftTextureIndex;
	int rightTextureIndex;
	int topTextureIndex;

public:
	CubeBlock(int all, string typeName) : CubeBlock(all, all, all, all, all, all, typeName) {};
	CubeBlock(int top, int front, int right, int back, int left, int bottom, string typeName);
	void build(BlockContext& context) override;
};

/*class SelectionBlock : public Block
{
private:
	float resistance = 10.0f;
	float curTime = 0.0f;
public:
	SelectionBlock(float breakage);
};*/

class AirBlock : public CubeBlock
{
public:
	AirBlock();
};

#endif // BLOCK_H
