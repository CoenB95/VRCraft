#pragma once

class CubeBlock;

class AirBlock : public CubeBlock {
public:
	AirBlock() : CubeBlock(-1, true) { };
};

class CobblestoneBlock : public CubeBlock {
public:
	CobblestoneBlock() : CubeBlock(Block::TILES_WIDTH_COUNT) { };
};

class DirtBlock : public CubeBlock {
public:
	DirtBlock() : CubeBlock(2) { };
	void randomTick() override;
};

class GrassBlock : public CubeBlock {
public:
	GrassBlock() : CubeBlock(0, 3, 3, 3, 3, 2) { };
	void randomTick() override;
};

class StoneBlock : public CubeBlock {
public:
	StoneBlock() : CubeBlock(1) { };
};
