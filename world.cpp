#include <cmath>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

#include "block.h"
#include "chunk.h"
#include "gameobject.h"
#include "world.h"

World::World(vec3 worldSize, vec3 chunkSize, vec3 blockSize) : worldSize(worldSize), chunkSize(chunkSize), blockSize(blockSize) {
	for (int y = 0; y < worldSize.y; y++)
	{
		for (int z = 0; z < worldSize.z; z++)
		{
			for (int x = 0; x < worldSize.x; x++)
			{
				Chunk* chunk = new Chunk(chunkSize, vec3(1, 1, 1));
				chunk->position = vec3(x * chunkSize.x * blockSize.x, y * chunkSize.y * blockSize.y, z * chunkSize.z * blockSize.z);
				chunks.push_back(chunk);
			}
		}
	}
}

void World::build() {
	//vertices.clear();
	for (GLuint i = 0; i < chunks.size(); i++) {
		ChunkContext context = getAdjacentChunks(chunks[i]->position);
		chunks[i]->build(context);
		//vertices.insert(vertices.end(), blocks[i]->vertices.begin(), blocks[i]->vertices.end());
	}
};

void World::draw(const glm::mat4& projectionMatrix, const glm::mat4& modelViewMatrix) {
	GameObject::draw(projectionMatrix, modelViewMatrix);

	for (int i = 0; i < chunks.size(); i++)
		chunks[i]->draw(projectionMatrix, modelViewMatrix);
}

BlockContext World::getAdjacentBlocks(vec3 positionInWorld) {
	Chunk* centerChunk = getChunk(positionInWorld);
	if (centerChunk == nullptr)
		return BlockContext(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

	ChunkContext chunkContext = getAdjacentChunks(positionInWorld);
	return centerChunk->getAdjacentBlocks(chunkContext, positionInWorld / chunkSize);
}

ChunkContext World::getAdjacentChunks(vec3 positionInWorld) {
	Chunk* centerChunk = getChunk(positionInWorld);
	if (centerChunk == nullptr)
		return ChunkContext(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);

	ChunkContext context = ChunkContext(
		getChunk(centerChunk->position + vec3(+0, +(chunkSize.y * blockSize.y), +0)),
		getChunk(centerChunk->position + vec3(+0, +0, -(chunkSize.z * blockSize.z))),
		getChunk(centerChunk->position + vec3(+(chunkSize.x * blockSize.x), +0, +0)),
		getChunk(centerChunk->position + vec3(+0, +0, +(chunkSize.z * blockSize.z))),
		getChunk(centerChunk->position + vec3(-(chunkSize.x * blockSize.x), +0, +0)),
		getChunk(centerChunk->position + vec3(+0, -(chunkSize.y * blockSize.y), +0))
	);
	return context;
}

Block* World::getBlock(vec3 positionInWorld) {
	Chunk* chunk = getChunk(positionInWorld);
	if (chunk == nullptr)
		return nullptr;

	return chunk->getBlock(positionInWorld);
}

Chunk* World::getChunk(vec3 positionInWorld) {
	int index = getChunkIndex(positionInWorld);

	if (index < 0)
		return nullptr;

	return chunks[index];
}

int World::getChunkIndex(vec3 positionInWorld) {
	vec3 chunkPosition = vec3(
		roundf(positionInWorld.x / blockSize.x / chunkSize.x),
		roundf(positionInWorld.y / blockSize.y / chunkSize.y),
		roundf(positionInWorld.z / blockSize.z / chunkSize.z));

	if (chunkPosition.x < 0 || chunkPosition.y < 0 || chunkPosition.z < 0)
		return -1;

	if (chunkPosition.x >= worldSize.x || chunkPosition.y >= worldSize.y || chunkPosition.z >= worldSize.z)
		return -1;

	return (int)(chunkPosition.x + chunkPosition.z * worldSize.x + chunkPosition.y * worldSize.x * worldSize.z);
}

Chunk** World::getChunkPtr(vec3 positionInWorld) {
	int index = getChunkIndex(positionInWorld);

	if (index < 0)
		return nullptr;

	return &chunks[index];
}

void World::loadTextures() {
	for (int i = 0; i < chunks.size(); i++)
		chunks[i]->loadTextures();
}

void World::randomTick() {
	for (int i = 0; i < 5; i++) {
		int randomTickChunkIndex = rand() % chunks.size();
		Chunk* chunk = chunks[randomTickChunkIndex];
		ChunkContext chunkContext = getAdjacentChunks(chunk->position);

		chunk->randomTick(chunkContext);
	}
}
