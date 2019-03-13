#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES

#include <GL/glew.h>
#include <cstdio>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <list>
#include <thread>
#include <vector>
#include <VrLib/Application.h>
#include <VrLib/Device.h>
#include <VrLib/gl/shader.h>
#include <VrLib/Log.h>
#include <VrLib/Texture.h>

#include "vr_craft.h"

#include "block.h"
#include "blocks.h"
#include "followcomponent.h"
#include "mob.h"
#include "model.h"
#include "raycast.h"
#include "texturedrawcomponent.h"
#include "world.h"

using namespace std;
using namespace glm;

using vrlib::Log;
using vrlib::logger;

vector<GameObject*> gameObjects3D;

World* world;
Block* player;

VrCraft::VrCraft() {
	clearColor = vec4(0.0f, 0.5f, 0.9f, 1.0f);
}

void VrCraft::init() {
	secondaryWandInput.init("buttonLeftTouch");
	secondaryWandPosition.init("WandPositionLeft");

	Shaders::setupDefaultShaders();

	world = new World(vec3(4, 4, 4), vec3(16, 16, 16), vec3(1, 1, 1));
	world->position = vec3(0, -3*16*1, 0);
	world->loadTextures();

	builderThread = new thread([this]() {
		while (true)
		{
			logger << "Building world..." << Log::newline;
			world->build();
			logger << "World build" << Log::newline;
			this_thread::sleep_for(0.5s);
		}
	});

	//player = new Steve(*world);
	player = new CobblestoneBlock();
	player->addComponent(new TextureDrawComponent("data/VrCraft/textures/terrain.png"));
	player->updateContext(new BlockContext());
	player->build();

	logger << "Initialized" << Log::newline;

	gameObjects3D.push_back(world);
	gameObjects3D.push_back(player);
}

//void display()
void VrCraft::draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix) {
	//glEnable(GL_CULL_FACE);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	
	for (GameObject* object : gameObjects3D)
		object->draw(projectionMatrix, modelViewMatrix);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void VrCraft::preFrame(double frameTime, double totalTime) {
	float elapsedSeconds = (float)(frameTime / 1000.0);

	mat4 k = secondaryWandPosition.getData();
	vec3 axis = vec3((k * vec4(0, 0, 1, 1)) - (k * vec4(0, 0, 0, 1)));
	player->orientation = angleAxis(0.0f, axis);
	player->position = vec3(k * vec4(0, 0, 0, 1));
	player->scale = vec3(0.1f, 0.1f, 2.0f);

	for (GameObject* object : gameObjects3D)
		object->update(elapsedSeconds);
}