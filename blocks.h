#pragma once

#define XY(x, y) x + y * w

class CubeBlock;

static const int w = Block::TILES_WIDTH_COUNT;

class AirBlock : public CubeBlock {
public:
	AirBlock(vec3 blockSize = vec3(1, 1, 1)) : CubeBlock(-1, blockSize, true) { };
};

class CobblestoneBlock : public CubeBlock {
public:
	CobblestoneBlock(vec3 blockSize = vec3(1, 1, 1)) : CubeBlock(Block::TILES_WIDTH_COUNT, blockSize) { };
};

class DirtBlock : public CubeBlock {
public:
	DirtBlock(vec3 blockSize = vec3(1, 1, 1)) : CubeBlock(2, blockSize) { };
	void randomTick() override;
};

class GrassBlock : public CubeBlock {
public:
	GrassBlock(vec3 blockSize = vec3(1, 1, 1)) : CubeBlock(0, 3, 3, 3, 3, 2, blockSize) { };
	void randomTick() override;
};

class PumpkinBlock : public CubeBlock {
public:
	PumpkinBlock(vec3 blockSize = vec3(1, 1, 1)) : CubeBlock(XY(6, 7), XY(7, 7), XY(6, 7), XY(6, 7), XY(6, 7), XY(6, 7), blockSize) { };
};

class StoneBlock : public CubeBlock {
public:
	StoneBlock(vec3 blockSize = vec3(1, 1, 1)) : CubeBlock(1, blockSize) { };
};
