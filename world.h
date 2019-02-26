#pragma once

#include <vector>
#include "gameobject.h"
#include "chunk.h"

using namespace std;

class World : public GameObject {
private:
	vector<Chunk*> chunks;
	vec3 blockSize;
	vec3 chunkSize;
	vec3 worldSize;

	int getChunkIndex(vec3 positionInWorld);

public:
	World(vec3 worldSize, vec3 chunkSize, vec3 blockSize);

	void build();
	void draw(const glm::mat4& projectionMatrix, const glm::mat4& modelViewMatrix) override;
	BlockContext getAdjacentBlocks(vec3 positionInWorld);
	ChunkContext getAdjacentChunks(vec3 positionInWorld);
	Block* getBlock(vec3 positionInWorld);
	Chunk* getChunk(vec3 positionInWorld);
	Chunk** getChunkPtr(vec3 positionInWorld);
	void loadTextures();
	void randomTick();
	void update(float elapsedSeconds) override;
};