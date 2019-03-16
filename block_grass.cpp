#include <iostream>
#include <sstream>

#include "block.h"
#include "blocks.h"
#include "chunk.h"

void GrassBlock::randomTick() {
	if (*context->up != nullptr && !(*context->up)->isTransparent) {
		DirtBlock* dirt = new DirtBlock();
		dirt->isTransparent = isTransparent;
		parentChunk->setBlock(position, dirt);
	}
}

void DirtBlock::randomTick() {
	//Todo: check conditions for grass to grow.
}
