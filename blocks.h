#pragma once

class CubeBlock;

class CobblestoneBlock : public CubeBlock {
public:
	CobblestoneBlock() : CubeBlock(Block::TILES_WIDTH_COUNT, "Cobblestone") { };
};

class DirtBlock : public CubeBlock {
public:
	DirtBlock() : CubeBlock(2, "Dirt") { };
	void randomTick() override;
};

class GrassBlock : public CubeBlock {
public:
	GrassBlock() : CubeBlock(0, 3, 3, 3, 3, 2, "Grass") { };
	void randomTick() override;
};

class StoneBlock : public CubeBlock {
public:
	StoneBlock() : CubeBlock(1, "Stone") { };
};
