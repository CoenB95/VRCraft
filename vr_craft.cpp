#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif // !_USE_MATH_DEFINES

#include <GL/glew.h>
#include <cstdio>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <list>
#include <mutex>
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
#include "despawncomponent.h"
#include "followcomponent.h"
#include "mob.h"
#include "model.h"
#include "physics.h"
#include "physics_nvidia.h"
#include "physicscomponent.h"
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
Block* wand;
Block* testBlock;
float physicsWait;

VrCraft::VrCraft() {
	clearColor = vec4(0.0f, 0.5f, 0.9f, 1.0f);
}

void VrCraft::init() {
	secondaryWandInput.init("buttonLeftTrigger");
	secondaryWandPosition.init("WandPositionLeft");

	Shaders::setupDefaultShaders();

	world = new World(worldSize, chunkSize, blockSize);
	world->loadTextures();

	//player = new Steve(*world);
	player = new CobblestoneBlock();
	player->addComponent(new TextureDrawComponent("data/VrCraft/textures/terrain.png"));
	player->addComponent(new SpinComponent(10.0f));
	player->updateContext(new BlockContext());
	player->scale = vec3(0.2f, 0.01f, 0.2f);
	player->buildStandalone();

	wand = new PumpkinBlock();
	wand->addComponent(new TextureDrawComponent("data/VrCraft/textures/terrain.png"));
	wand->updateContext(new BlockContext());
	wand->buildStandalone();

	testBlock = new CobblestoneBlock(vec3(0.2f, 0.2f, 0.2f));
	testBlock->addComponent(new TextureDrawComponent("data/VrCraft/textures/terrain.png"));
	testBlock->addComponent(new SpinComponent(10.0f));
	testBlock->updateContext(new BlockContext());
	testBlock->position = vec3(0.5f * blockSize.x, 0.5f * blockSize.y, 0.5f * blockSize.z);
	testBlock->buildStandalone();

	//Make random more random.
	time_t currentTime;
	time(&currentTime);
	srand(currentTime);

	builderThread = new thread([this]() {
		loading = true;
		logger << "Populating world from seed..." << Log::newline;
		world->populateFromSeed(worldSeed);
		logger << "World build" << Log::newline;
		loading = false;
		
		spawnPlayer();

		world->buildStandalone();
		initPhysics();

		while (true) {
			world->randomTick();
			world->buildStandalone();
			this_thread::sleep_for(0.5s);
		}
	});

	logger << "Initialized" << Log::newline;

	gameObjects3D.push_back(world);
	gameObjects3D.push_back(player);
	gameObjects3D.push_back(wand);
	gameObjects3D.push_back(testBlock);
}

//void display()
void VrCraft::draw(const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix) {
	//glEnable(GL_CULL_FACE);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	
	mat4 modelMatrix = mat4();
	modelMatrix = glm::translate(modelMatrix, vec3(-player->position.x, -player->position.y, -player->position.z));
	for (GameObject* object : gameObjects3D)
		object->draw(projectionMatrix, viewMatrix, modelMatrix);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void VrCraft::preFrame(double frameTime, double totalTime) {
	float elapsedSeconds = (float)(frameTime / 1000.0);

	if (physicsWorld != nullptr) {
		physicsWorld->onUpdate(elapsedSeconds);

		physicsWait -= elapsedSeconds;
		if (secondaryWandInput.getData() == vrlib::DigitalState::OFF)
			physicsWait = -1;
		if (physicsWait < 0 && secondaryWandInput.getData() == vrlib::DigitalState::ON) {
			physicsWait = 0.1f;
			throwBlock();
		}
	}

	mat4 k = secondaryWandPosition.getData();
	vec3 axis = vec3((k * vec4(0, 0, 1, 1)) - (k * vec4(0, 0, 0, 1)));
	wand->orientation = quat_cast(k);
	wand->position = player->position + vec3(k * vec4(0, 0, 0, 1));
	wand->scale = vec3(0.1f, 0.1f, 0.1f);

	{
		lock_guard<mutex> lock(updateMutex);
		for (GameObject* object : gameObjects3D)
			object->update(elapsedSeconds);
	}
}

void VrCraft::initPhysics() {
	lock_guard<mutex> lock(updateMutex);
	physicsWorld = new NvidiaPhysics();
	physicsWorld->setup(vec3(0.0f, -9.81f, 0.0f));
	for (Chunk* c : world->chunks) {
		auto m = physicsWorld->addMesh(c, true);
		if (m != nullptr)
			c->addComponent(new PhysicsComponent(m));
	}
}

void VrCraft::spawnPlayer() {
	//Spawn in center of world.
	vec2 pp = vec2(
		(0.5f * (worldSize.x * chunkSize.x * blockSize.x)),
		(0.5f * (worldSize.z * chunkSize.z * blockSize.z)));

	logger << "Trying to find spawn position at (" << pp.x << ";" << pp.y << ")" << Log::newline;
	Block* spawnPoint = world->tryFindArea(pp, vec3(1, 2, 1));
	if (spawnPoint != nullptr) {
		vec3 p = spawnPoint->globalPosition();
		p += vec3(0.5f * blockSize.x, 0, 0.5f * blockSize.z);
		logger << "Spawning @ (" << p.x << ";" << p.y << ";" << p.z << ")" << Log::newline;
		player->position = p;
	}
	else {
		logger << "Couldn't find valid spawn position" << Log::newline;
	}
}

void VrCraft::throwBlock() {
	Block* newBlock = new CobblestoneBlock(vec3(0.2f, 0.2f, 0.2f));
	newBlock->addComponent(new TextureDrawComponent("data/VrCraft/textures/terrain.png"));
	newBlock->updateContext(new BlockContext());
	newBlock->buildStandalone();
	newBlock->position = wand->position;

	PhysicsRigidBody* body = physicsWorld->addBox(newBlock, newBlock->getBlockSize());
	body->setCollisionListener([this, newBlock](PhysicsRigidBody* other) {
		Block* otherBlock = dynamic_cast<Block*>(other->getObject());
		if (otherBlock != nullptr && otherBlock->parentChunk != nullptr) {
			world->setBlock(newBlock->position, newBlock);
		}
	});

	newBlock->addComponent(new PhysicsComponent(body));
	newBlock->addComponent(new DespawnComponent(world, 2.0f));
	body->addForce(wand->orientation * vec3(0, 0, -500));
	//gameObjects3D.push_back(newBlock);
	world->addChild(newBlock);
}