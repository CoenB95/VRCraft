#pragma once

#include <vector>
#include "gameobject.h"
#include "chunk.h"

using namespace std;

class BlockContext;

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
	void draw(const mat4& projectionMatrix, const mat4& modelViewMatrix, const mat4& parentModelMatrix) override;
	BlockContext* getAdjacentBlocks(vec3 positionInWorld);
	ChunkContext* getAdjacentChunks(vec3 positionInWorld);
	Block* getBlock(vec3 positionInWorld);
	Chunk* getChunk(vec3 positionInWorld);
	Chunk** getChunkPtr(vec3 positionInWorld);
	void loadTextures();
	void randomTick();
	void update(float elapsedSeconds) override;
};