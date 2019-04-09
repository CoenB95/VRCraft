#pragma once

#include <vector>
#include "gameobjectgroup.h"
#include "chunk.h"

using namespace std;

class BlockContext;

class World : public GameObjectGroup {
public:
	vector<Chunk*> chunks;
private:
	vec3 blockSize;
	vec3 chunkSize;
	vec3 worldSize;

	int getChunkIndex(vec3 positionInWorld);

public:
	World(vec3 worldSize, vec3 chunkSize, vec3 blockSize);

	void build(vec3 offsetPosition) override;
	bool ensureArea(vec3 spawnPosition, vec3 areaSize);
	BlockContext* getAdjacentBlocks(vec3 positionInWorld);
	ChunkContext* getAdjacentChunks(vec3 positionInWorld);
	Block* getBlock(vec3 positionInWorld);
	Chunk* getChunk(vec3 positionInWorld);
	Chunk** getChunkPtr(vec3 positionInWorld);
	void loadTextures();
	void populateFromSeed(int worldSeed);
	void randomTick();
	void setBlock(vec3 positionInWorld, Block* newBlock);
	Block* tryFindArea(vec2 xzCoordsInWorld, vec3 areaSize);
};