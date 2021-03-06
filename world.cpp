#include <cmath>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "world.h"

#include "block.h"
#include "chunk.h"
#include "gameobject.h"
#include "gameobjectcomponent.h"

World::World(vec3 worldSize, vec3 chunkSize, vec3 blockSize) : worldSize(worldSize), chunkSize(chunkSize), blockSize(blockSize) {
	for (int worldIndexY = 0; worldIndexY < worldSize.y; worldIndexY++) {
		for (int worldIndexZ = 0; worldIndexZ < worldSize.z; worldIndexZ++) {
			for (int worldIndexX = 0; worldIndexX < worldSize.x; worldIndexX++) {
				vec3 pos = vec3(
					worldIndexX * chunkSize.x * blockSize.x,
					worldIndexY * chunkSize.y * blockSize.y,
					worldIndexZ * chunkSize.z * blockSize.z);
				Chunk* chunk = new Chunk(chunkSize, vec3(1, 1, 1));
				chunk->position = pos;
				//chunk->addComponent(new SpinComponent(10.0f));
				chunks.push_back(chunk);
				addChild(chunk);
			}
		}
	}
}

void World::build(vec3 offsetPosition) {
	for (GLuint i = 0; i < chunks.size(); i++) {
		if (chunks[i]->shouldRebuild()) {
			chunks[i]->updateContext(getAdjacentChunks(chunks[i]->position));
			chunks[i]->buildStandalone(false);
		}
	}
	GameObjectGroup::build(offsetPosition);
};

bool World::ensureArea(vec3 spawnPosition, vec3 areaSize) {
	for (int bx = 0; bx < areaSize.x; bx++) {
		for (int by = 0; by < areaSize.y; by++) {
			for (int bz = 0; bz < areaSize.z; bz++) {
				Block* b = getBlock(vec3(spawnPosition.x + bx, spawnPosition.y + by, spawnPosition.z + bz));
				if (b != nullptr && !b->isTransparent)
					return false;
			}
		}
	}
	return true;
}

BlockContext* World::getAdjacentBlocks(vec3 positionInWorld) {
	Chunk* centerChunk = getChunk(positionInWorld);
	if (centerChunk == nullptr)
		return new BlockContext();

	ChunkContext* chunkContext = getAdjacentChunks(positionInWorld);
	BlockContext* blockContext = centerChunk->getAdjacentBlocks(chunkContext, positionInWorld - centerChunk->position);
	return blockContext;
}

ChunkContext* World::getAdjacentChunks(vec3 positionInWorld) {
	Chunk* centerChunk = getChunk(positionInWorld);
	if (centerChunk == nullptr)
		return new ChunkContext();

	return new ChunkContext(
		getChunk(centerChunk->position + vec3(+0, +(chunkSize.y * blockSize.y), +0)),
		getChunk(centerChunk->position + vec3(+0, +0, +(chunkSize.z * blockSize.z))),
		getChunk(centerChunk->position + vec3(+(chunkSize.x * blockSize.x), +0, +0)),
		getChunk(centerChunk->position + vec3(+0, +0, -(chunkSize.z * blockSize.z))),
		getChunk(centerChunk->position + vec3(-(chunkSize.x * blockSize.x), +0, +0)),
		getChunk(centerChunk->position + vec3(+0, -(chunkSize.y * blockSize.y), +0))
	);
}

Block* World::getBlock(vec3 positionInWorld) {
	Chunk* chunk = getChunk(positionInWorld);
	if (chunk == nullptr)
		return nullptr;

	return chunk->getBlock(positionInWorld - chunk->position);
}

Chunk* World::getChunk(vec3 positionInWorld) {
	int index = getChunkIndex(positionInWorld);

	if (index < 0 || index >= chunks.size())
		return nullptr;

	return chunks[index];
}

int World::getChunkIndex(vec3 positionInWorld) {
	vec3 chunkPosition = vec3(
		floorf(positionInWorld.x / blockSize.x / chunkSize.x),
		floorf(positionInWorld.y / blockSize.y / chunkSize.y),
		floorf(positionInWorld.z / blockSize.z / chunkSize.z));

	if (chunkPosition.x < 0 || chunkPosition.y < 0 || chunkPosition.z < 0)
		return -1;

	if (chunkPosition.x >= worldSize.x || chunkPosition.y >= worldSize.y || chunkPosition.z >= worldSize.z)
		return -1;

	return (int)((chunkPosition.x) + (chunkPosition.z * worldSize.x) + (chunkPosition.y * worldSize.x * worldSize.z));
}

Chunk** World::getChunkPtr(vec3 positionInWorld) {
	int index = getChunkIndex(positionInWorld);

	if (index < 0)
		return nullptr;

	return &chunks[index];
}

void World::loadTextures() {
	for (GLuint i = 0; i < chunks.size(); i++)
		chunks[i]->loadTextures();
}

void World::populateFromSeed(int worldSeed) {
	for (int worldIndexY = 0; worldIndexY < worldSize.y; worldIndexY++) {
		for (int worldIndexZ = 0; worldIndexZ < worldSize.z; worldIndexZ++) {
			for (int worldIndexX = 0; worldIndexX < worldSize.x; worldIndexX++) {
				vec3 pos = vec3(
					worldIndexX * chunkSize.x * blockSize.x,
					worldIndexY * chunkSize.y * blockSize.y,
					worldIndexZ * chunkSize.z * blockSize.z);
				Chunk* chunk = getChunk(pos);
				chunk->populateFromSeed(worldSize, worldSeed);
			}
		}
	}
}

void World::randomTick() {
	if (chunks.empty())
		return;

	for (GLuint i = 0; i < 1; i++) {
		int randomTickChunkIndex = rand() % chunks.size();
		Chunk* chunk = chunks[randomTickChunkIndex];
		ChunkContext* chunkContext = getAdjacentChunks(chunk->position);
		chunk->updateContext(chunkContext);
		chunk->randomTick();
	}
}

void World::setBlock(vec3 positionInWorld, Block* newBlock) {
	Chunk* chunk = getChunk(positionInWorld);
	if (chunk == nullptr)
		return;

	chunk->setBlock(positionInWorld - chunk->position, newBlock);
}

Block* World::tryFindArea(vec2 xzCoordsInWorld, vec3 areaSize) {
	for (float y = 0.0f; y < worldSize.y * chunkSize.y * blockSize.y; y += blockSize.y) {
		Block* spawnBlock = getBlock(vec3(xzCoordsInWorld.x, y, xzCoordsInWorld.y));
		if (ensureArea(spawnBlock->globalPosition(), areaSize))
			return spawnBlock;
	}

	return nullptr;
}

/*void World::update(float elapsedSeconds) {
	GameObject::update(elapsedSeconds);
	randomTick();
	for (Chunk* chunk : chunks)
		chunk->update(elapsedSeconds);
}*/
