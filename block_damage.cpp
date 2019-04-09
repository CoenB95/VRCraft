#include "block.h"
#include "blocks.h"

void DamageBlock::setDamage(float value) {
	setTextureIndexes(XY((int)(value * 10), 15));
}