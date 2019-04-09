#include <glm/glm.hpp>

#include "block.h"
#include "blocks.h"

void DamageBlock::setDamage(float value) {
	int niveau = ((int)ceilf(value * 8) - 1) % 8;
	setTextureIndexes(niveau < 0 ? -1 : XY(niveau, 15));
}