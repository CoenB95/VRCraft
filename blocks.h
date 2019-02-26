#pragma once

class CubeBlock;

class CobblestoneBlock : public CubeBlock {
public:
	CobblestoneBlock() : CubeBlock(Block::TILES_WIDTH_COUNT, "Cobblestone") { };
};

class DirtBlock : public CubeBlock {
public:
	DirtBlock() : CubeBlock(2, "Dirt") { };
	Block* randomTick(BlockContext& adjacentBlocks) override;
};

class GrassBlock : public CubeBlock {
public:
	GrassBlock() : CubeBlock(0, 3, 3, 3, 3, 2, "Grass") { };
	Block* randomTick(BlockContext& adjacentBlocks) override;
};

class StoneBlock : public CubeBlock {
public:
	StoneBlock() : CubeBlock(1, "Stone") { };
};
