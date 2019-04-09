#include "block.h"
#include "blocks.h"
#include "spincomponent.h"
#include "texturedrawcomponent.h"

#include "player.h"

Player::Player() {
	feet = new CobblestoneBlock();
	feet->addComponent(new TextureDrawComponent("data/VrCraft/textures/terrain.png"));
	feet->addComponent(new SpinComponent(10.0f));
	((Block*)feet)->updateContext(new BlockContext());
	feet->scale = vec3(0.2f, 0.01f, 0.2f);
	feet->buildStandalone();
	addChild(feet);

	primaryHand = new PumpkinBlock();
	primaryHand->addComponent(new TextureDrawComponent("data/VrCraft/textures/terrain.png"));
	((Block*)primaryHand)->updateContext(new BlockContext());
	primaryHand->scale = vec3(0.1f, 0.1f, 0.1f);
	primaryHand->buildStandalone();
	addChild(primaryHand);
}