#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif // !STB_IMAGE_IMPLEMENTATION

#include <gl/glew.h>
#include <cstddef>
#include <iostream>
#include <vector>
#include <VrLib/Log.h>
#include <VrLib/Texture.h>

#define STB_PERLIN_IMPLEMENTATION

#include "block.h"
#include "blocks.h"
#include "chunk.h"
#include "gameobject.h"
#include "stack.h"
#include "stb_perlin.h"
#include "texturedrawcomponent.h"

using vrlib::Log;
using vrlib::logger;

Chunk::Chunk(vec3 chunkSize, vec3 blockSize) : chunkSize(chunkSize), blockSize(blockSize) {
	pivot = vec3(chunkSize.x * blockSize.x / 2, chunkSize.y * blockSize.y / 2, chunkSize.z * blockSize.z / 2);
}

void Chunk::build(vec3 offsetPosition) {
	//GameObject::build(offsetPosition);

	updateNewBlocks(context);

	vector<vrlib::gl::VertexP3N3T2> newVertices;
	newVertices.reserve(blocks.size() * 36);

	for (GLuint i = 0; i < blocks.size(); i++) {
		if (context != nullptr && blocks[i]->shouldRebuild()) {
			if (blocks[i]->needsContext())
				blocks[i]->updateContext(getAdjacentBlocks(context, blocks[i]->position));
			blocks[i]->buildEmbedded(offsetPosition);
		}
		newVertices.insert(newVertices.end(), blocks[i]->vertices.begin(), blocks[i]->vertices.end());
		if (blocks[i]->shouldRebuild())
			notifyDirty();
	}

	{
		lock_guard<mutex> lock(verticesMutex);
		vertices.clear();
		vertices.insert(vertices.end(), newVertices.begin(), newVertices.end());
	}

	GameObject::build(offsetPosition);
}

BlockContext* Chunk::getAdjacentBlocks(ChunkContext* chunkContext, vec3 positionInChunk) {
	Block* centerBlock = getBlock(positionInChunk);
	if (centerBlock == nullptr)
		return new BlockContext();

	BlockContext* context = new BlockContext();
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			for (int z = 0; z < 3; z++) {
				context->surroundings[x][y][z] = getBlock(centerBlock->position + vec3(x-1, y-1, z-1));
			}
		}
	}

	if (*context->up == nullptr && chunkContext != nullptr && *chunkContext->up != nullptr)
		*context->up = (*chunkContext->up)->getBlock(centerBlock->position + vec3(+0, -chunkSize.y + 1, +0));
	if (*context->south == nullptr && chunkContext != nullptr && *chunkContext->south != nullptr)
		*context->south = (*chunkContext->south)->getBlock(centerBlock->position + vec3(+0, +0, -chunkSize.z + 1));
	if (*context->east == nullptr && chunkContext != nullptr && *chunkContext->east != nullptr)
		*context->east = (*chunkContext->east)->getBlock(centerBlock->position + vec3(-chunkSize.x + 1, +0, +0));
	if (*context->north == nullptr && chunkContext != nullptr && *chunkContext->north != nullptr)
		*context->north = (*chunkContext->north)->getBlock(centerBlock->position + vec3(+0, +0, +chunkSize.z - 1));
	if (*context->west == nullptr && chunkContext != nullptr && *chunkContext->west != nullptr)
		*context->west = (*chunkContext->west)->getBlock(centerBlock->position + vec3(+chunkSize.x - 1, +0, +0));
	if (*context->down == nullptr && chunkContext != nullptr && *chunkContext->down != nullptr)
		*context->down = (*chunkContext->down)->getBlock(centerBlock->position + vec3(+0, +chunkSize.y - 1, +0));

	return context;
}

Block* Chunk::getBlock(vec3 positionInChunk) {
	GLuint index = getBlockIndex(positionInChunk);

	if (index < 0 || index >= blocks.size())
		return nullptr;

	return blocks[index];
}

int Chunk::getBlockIndex(vec3 positionInChunk) {
	vec3 blockPosition = vec3(
		floorf(positionInChunk.x / blockSize.x),
		floorf(positionInChunk.y / blockSize.y),
		floorf(positionInChunk.z / blockSize.z));

	if (blockPosition.x < 0 || blockPosition.y < 0 || blockPosition.z < 0)
		return -1;

	if (blockPosition.x >= chunkSize.x || blockPosition.y >= chunkSize.y || blockPosition.z >= chunkSize.z)
		return -1;

	return (int)((blockPosition.x) + (blockPosition.z * chunkSize.x) + (blockPosition.y * chunkSize.x * chunkSize.z));
}

Block** Chunk::getBlockPtr(vec3 positionInChunk) {
	int index = getBlockIndex(positionInChunk);

	if (index < 0)
		return nullptr;

	return &blocks[index];
}

bool Chunk::isBlockTransparent(Block* block)
{
	return block != nullptr && block->isTransparent;
}

void Chunk::loadTextures() {
	TextureDrawComponent* component = new TextureDrawComponent("data/VrCraft/textures/terrain.png");
	addComponent(component);
}

void Chunk::populateFromSeed(vec3 worldSize, int seed) {
	for (int chunkIndexY = 0; chunkIndexY < chunkSize.y; chunkIndexY++) {
		for (int chunkIndexZ = 0; chunkIndexZ < chunkSize.z; chunkIndexZ++) {
			for (int chunkIndexX = 0; chunkIndexX < chunkSize.x; chunkIndexX++) {
				Block* block;

				vec3 blockPositionInChunk = vec3(
					chunkIndexX * blockSize.x,
					chunkIndexY * blockSize.y,
					chunkIndexZ * blockSize.z
				);
				
				vec3 blockPositionInWorld = vec3(
					position.x * worldSize.x + blockPositionInChunk.x,
					position.y * worldSize.y + blockPositionInChunk.y,
					position.z * worldSize.z + blockPositionInChunk.z
				);

				float noiseX = (position.x + chunkIndexX * blockSize.x) / (worldSize.x * chunkSize.x * blockSize.x) * 2 - 1;
				float noiseY = (position.y + chunkIndexY * blockSize.y) / (worldSize.y * chunkSize.y * blockSize.y);
				float noiseZ = (position.z + chunkIndexZ * blockSize.z) / (worldSize.z * chunkSize.z * blockSize.z) * 2 - 1;

				//float noise1 = stb_perlin_noise3(noiseZ, seed, 0, 0, 0, 0);
				//float noise = stb_perlin_noise3(noiseX, noiseY, noise1, 0, 0, 0);
				float heightNoise = (stb_perlin_noise3_seed(noiseX, 0, noiseZ, 0, 0, 0, seed) + 1.0f) / 2.0f;
				
				if (noiseY < heightNoise)
					block = new GrassBlock();
				else
					block = new AirBlock();

				/*if (noise > 0.7)
					block = new CobblestoneBlock();
				else if (noise > 0.2)
					block = new StoneBlock();
				else if (noise > 0)
					block = new GrassBlock();
				else
					block = new AirBlock();*/

				block->parentChunk = this;
				block->position = blockPositionInChunk;
				blocks.push_back(block);
			}
		}
	}
}

void Chunk::randomTick() {
	if (blocks.empty())
		return;

	for (int i = 0; i < 5; i++) {
		int randomTickBlockIndex = rand() % blocks.size();
		Block* block = getBlock(blocks[randomTickBlockIndex]->position);
		BlockContext* blockContext = getAdjacentBlocks(context, block->position);
		block->updateContext(blockContext);
		block->randomTick();
	}
}

void Chunk::setBlock(vec3 positionInChunk, Block* newBlock) {
	newBlock->position = positionInChunk;
	newBlock->parentChunk = this;
	{
		lock_guard<mutex> lock(verticesMutex);
		newBlocks.push_back(newBlock);
	}
	notifyDirty();
}

void Chunk::update(float elapsedSeconds) {
	GameObject::update(elapsedSeconds);
}

void Chunk::updateAdjacentBlocks(ChunkContext* chunkContext, vec3 positionInChunk) {
	BlockContext* context = getAdjacentBlocks(chunkContext, positionInChunk);
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			for (int z = 0; z < 3; z++) {
				Block* b = context->surroundings[x][y][z];
				if (b != nullptr)
					b->notifyDirty();
			}
		}
	}
}

void Chunk::updateContext(ChunkContext* chunkContext) {
	if (context == nullptr) {
		delete context;
	}

	context = chunkContext;
}

void Chunk::updateNewBlocks(ChunkContext* chunkContext) {
	lock_guard<mutex> lock(verticesMutex);
	for (GLuint i = 0; i < newBlocks.size(); i++) {
		Block** block = getBlockPtr(newBlocks[i]->position);
		if ((*block)->isTransparent != newBlocks[i]->isTransparent) {
			//Transparency changed: Either block sides may have been revealed or covered up.
			updateAdjacentBlocks(chunkContext, newBlocks[i]->position);
		}
		delete *block;
		*block = newBlocks[i];
	}
	newBlocks.clear();
}