#define BOTTOM_LEFT		0, 0
#define BOTTOM_RIGHT	1, 0
#define TOP_LEFT		0, 1
#define TOP_RIGHT		1, 1

#define TEX_POS(a)		vec2((a % TILES_WIDTH_COUNT), (TILES_HEIGHT_COUNT - 1 - a / TILES_WIDTH_COUNT))

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES

#include <cmath>
#include <GL/glew.h>
#include <string>
#include <sstream>
#include <vector>
#include <VrLib/Log.h>
#include <VrLib/gl/Vertex.h>

#include "block.h"
#include "color.h"
#include "gameobject.h"
#include "gameobjectcomponent.h"

using namespace glm;
using namespace std;

using vrlib::Log;
using vrlib::logger;

const int Block::TILES_HEIGHT_COUNT = 16;
const int Block::TILES_WIDTH_COUNT = 16;
const int Block::TILE_COUNT = TILES_WIDTH_COUNT * TILES_HEIGHT_COUNT;
const GLfloat Block::TILE_SIZE = 1.0f / 256.0f * TILES_WIDTH_COUNT;
const GLfloat Block::SCALE_BLOCK = 1.0f;
const GLfloat Block::SCALE_BLOCK_OVERLAY = 1.001f;
const GLfloat Block::SCALE_ITEM = 0.3f;

Block::Block(string typeName) : GameObject(), typeName(typeName) {

}

string Block::getPositionString() const
{
	stringstream ss;
	ss << "x=" << position.x << ", y=" << position.y << ", z=" << position.z;
	return ss.str();
}

vec3 Block::globalPosition() {
	return parentChunk == nullptr ? position : (parentChunk->globalPosition() + position);
}

string Block::toString() const {
	stringstream ss;
	ss << (typeName) << "{" << getPositionString() << "}";
	return ss.str();
}

void Block::updateContext(BlockContext* blockContext) {
	if (context == nullptr) {
		delete context;
	}

	context = blockContext;
}

CubeBlock::CubeBlock(int top, int front, int right, int back, int left, int bottom, string typeName, bool transparent) :
	Block(typeName),
	topTextureIndex(top),
	frontTextureIndex(front),
	rightTextureIndex(right),
	backTextureIndex(back),
	leftTextureIndex(left),
	bottomTextureIndex(bottom)
{
	isTransparent = transparent;
	pivot = vec3(0.5f, 0.5f, 0.5f);
}

void CubeBlock::build(vec3 offsetPosition) {
	GameObject::build(offsetPosition);

	if (context == nullptr) {
		//logger << "Building block failed: no context." << Log::newline;
		notifyDirty();
		return;
	}

	{
		lock_guard<mutex> lock(verticesMutex);

		vec3 pos = offsetPosition;
		vec2 texPos;
		vec2 texScl(TILE_SIZE, TILE_SIZE);
		vertices.clear();

		if ((*context->up == nullptr || (*context->up)->isTransparent) && topTextureIndex >= 0) {
			texPos = TEX_POS(topTextureIndex);
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(1, 1, 0), vec3(0, 1, 0), texScl * (texPos + vec2(TOP_RIGHT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(0, 1, 0), vec3(0, 1, 0), texScl * (texPos + vec2(TOP_LEFT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(0, 1, 1), vec3(0, 1, 0), texScl * (texPos + vec2(BOTTOM_LEFT))));

			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(1, 1, 0), vec3(0, 1, 0), texScl * (texPos + vec2(TOP_RIGHT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(0, 1, 1), vec3(0, 1, 0), texScl * (texPos + vec2(BOTTOM_LEFT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(1, 1, 1), vec3(0, 1, 0), texScl * (texPos + vec2(BOTTOM_RIGHT))));
		}

		if ((*context->south == nullptr || (*context->south)->isTransparent) && frontTextureIndex >= 0) {
			texPos = TEX_POS(frontTextureIndex);
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(1, 1, 1), vec3(0, 0, 1), texScl * (texPos + vec2(TOP_RIGHT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(0, 1, 1), vec3(0, 0, 1), texScl * (texPos + vec2(TOP_LEFT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(0, 0, 1), vec3(0, 0, 1), texScl * (texPos + vec2(BOTTOM_LEFT))));

			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(1, 1, 1), vec3(0, 0, 1), texScl * (texPos + vec2(TOP_RIGHT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(0, 0, 1), vec3(0, 0, 1), texScl * (texPos + vec2(BOTTOM_LEFT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(1, 0, 1), vec3(0, 0, 1), texScl * (texPos + vec2(BOTTOM_RIGHT))));
		}

		if ((*context->east == nullptr || (*context->east)->isTransparent) && rightTextureIndex >= 0) {
			texPos = TEX_POS(rightTextureIndex);
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(1, 1, 0), vec3(1, 0, 0), texScl * (texPos + vec2(TOP_RIGHT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(1, 1, 1), vec3(1, 0, 0), texScl * (texPos + vec2(TOP_LEFT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(1, 0, 1), vec3(1, 0, 0), texScl * (texPos + vec2(BOTTOM_LEFT))));

			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(1, 1, 0), vec3(1, 0, 0), texScl * (texPos + vec2(TOP_RIGHT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(1, 0, 1), vec3(1, 0, 0), texScl * (texPos + vec2(BOTTOM_LEFT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(1, 0, 0), vec3(1, 0, 0), texScl * (texPos + vec2(BOTTOM_RIGHT))));
		}

		if ((*context->north == nullptr || (*context->north)->isTransparent) && backTextureIndex >= 0) {
			texPos = TEX_POS(backTextureIndex);
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(0, 1, 0), vec3(0, 0, -1), texScl * (texPos + vec2(TOP_RIGHT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(1, 1, 0), vec3(0, 0, -1), texScl * (texPos + vec2(TOP_LEFT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(1, 0, 0), vec3(0, 0, -1), texScl * (texPos + vec2(BOTTOM_LEFT))));

			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(0, 1, 0), vec3(0, 0, -1), texScl * (texPos + vec2(TOP_RIGHT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(1, 0, 0), vec3(0, 0, -1), texScl * (texPos + vec2(BOTTOM_LEFT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(0, 0, 0), vec3(0, 0, -1), texScl * (texPos + vec2(BOTTOM_RIGHT))));
		}

		if ((*context->west == nullptr || (*context->west)->isTransparent) && leftTextureIndex >= 0) {
			texPos = TEX_POS(leftTextureIndex);
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(0, 1, 1), vec3(-1, 0, 0), texScl * (texPos + vec2(TOP_RIGHT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(0, 1, 0), vec3(-1, 0, 0), texScl * (texPos + vec2(TOP_LEFT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(0, 0, 0), vec3(-1, 0, 0), texScl * (texPos + vec2(BOTTOM_LEFT))));

			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(0, 1, 1), vec3(-1, 0, 0), texScl * (texPos + vec2(TOP_RIGHT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(0, 0, 0), vec3(-1, 0, 0), texScl * (texPos + vec2(BOTTOM_LEFT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(0, 0, 1), vec3(-1, 0, 0), texScl * (texPos + vec2(BOTTOM_RIGHT))));
		}

		if ((*context->down == nullptr || (*context->down)->isTransparent) && bottomTextureIndex >= 0) {
			texPos = TEX_POS(bottomTextureIndex);
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(1, 0, 1), vec3(0, -1, 0), texScl * (texPos + vec2(TOP_RIGHT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(0, 0, 1), vec3(0, -1, 0), texScl * (texPos + vec2(TOP_LEFT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(0, 0, 0), vec3(0, -1, 0), texScl * (texPos + vec2(BOTTOM_LEFT))));

			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(1, 0, 1), vec3(0, -1, 0), texScl * (texPos + vec2(TOP_RIGHT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(0, 0, 0), vec3(0, -1, 0), texScl * (texPos + vec2(BOTTOM_LEFT))));
			vertices.push_back(vrlib::gl::VertexP3N3T2(pos + vec3(1, 0, 0), vec3(0, -1, 0), texScl * (texPos + vec2(BOTTOM_RIGHT))));
		}
	}

	//Cleanup context
	delete context;
	context = nullptr;
}

/*SelectionBlock::SelectionBlock(float breakage) : Block(
	new TexturedBlockSide((GLint)(breakage * 10) - 1, 15, 1, 1, Color4f(1, 1, 1, (GLint)(breakage * 10) < 1 ? 0 : 1)),
	new TexturedBlockSide((GLint)(breakage * 10) - 1, 15, 1, 1, Color4f(1, 1, 1, (GLint)(breakage * 10) < 1 ? 0 : 1)),
	new TexturedBlockSide((GLint)(breakage * 10) - 1, 15, 1, 1, Color4f(1, 1, 1, (GLint)(breakage * 10) < 1 ? 0 : 1)),
	new TexturedBlockSide((GLint)(breakage * 10) - 1, 15, 1, 1, Color4f(1, 1, 1, (GLint)(breakage * 10) < 1 ? 0 : 1)),
	new TexturedBlockSide((GLint)(breakage * 10) - 1, 15, 1, 1, Color4f(1, 1, 1, (GLint)(breakage * 10) < 1 ? 0 : 1)),
	new TexturedBlockSide((GLint)(breakage * 10) - 1, 15, 1, 1, Color4f(1, 1, 1, (GLint)(breakage * 10) < 1 ? 0 : 1)),
	"Breakage ",
	SCALE_BLOCK_OVERLAY)
{

}*/
