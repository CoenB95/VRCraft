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
	//void draw(const mat4& projectionMatrix, const mat4& modelViewMatrix, const mat4& parentModelMatrix) override;
	BlockContext* getAdjacentBlocks(vec3 positionInWorld);
	ChunkContext* getAdjacentChunks(vec3 positionInWorld);
	Block* getBlock(vec3 positionInWorld);
	Chunk* getChunk(vec3 positionInWorld);
	Chunk** getChunkPtr(vec3 positionInWorld);
	void loadTextures();
	void populateFromSeed(int worldSeed);
	void randomTick();
	Block* tryFindArea(vec2 xzCoordsInWorld, vec3 areaSize);
	//void update(float elapsedSeconds) override;
};