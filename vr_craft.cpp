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
#include "player.h"
#include "raycast.h"
#include "spincomponent.h"
#include "texturedrawcomponent.h"
#include "world.h"

using namespace std;
using namespace glm;

using vrlib::Log;
using vrlib::logger;

vector<GameObject*> gameObjects3D;

Block* testBlock;

float damage = 0.0f;
float physicsWait;

int curShaderIndex = 5;

VrCraft::VrCraft() {
	clearColor = vec4(0.0f, 0.5f, 0.9f, 1.0f);
}

void VrCraft::init() {
	primaryWandMenu.init("buttonRightMenu");
	primaryWandPosition.init("WandPosition");
	primaryWandTouch.init("buttonRightTouch");
	primaryWandTouchPosition.init("RightThumbPos");
	primaryWandTrigger.init("buttonRightTrigger");
	secondaryWandPosition.init("WandPositionLeft");
	secondaryWandTouch.init("buttonLeftTouch");
	secondaryWandTouchPosition.init("LeftThumbPos");
	secondaryWandTrigger.init("buttonLeftTrigger");

	Shaders::setupDefaultShaders();
	shadowMapFbo = new vrlib::gl::FBO(1024 * 8, 1024 * 8, false, 0, true);

	world = new World(worldSize, chunkSize, blockSize);
	world->loadTextures();

	player = new Player();

	pickHighlight = new DamageBlock();
	pickHighlight->addComponent(new TextureDrawComponent("data/VrCraft/textures/terrain.png"));
	pickHighlight->updateContext(new BlockContext());
	pickHighlight->scale = vec3(1, 1, 1) * Block::SCALE_BLOCK_OVERLAY;
	pickHighlight->pivot = (pickHighlight->scale - vec3(1, 1, 1)) * 0.5f;
	pickHighlight->buildStandalone();

	testBlock = new CobblestoneBlock(vec3(0.2f, 0.2f, 0.2f));
	testBlock->addComponent(new TextureDrawComponent("data/VrCraft/textures/terrain.png"));
	testBlock->addComponent(new SpinComponent(10.0f));
	testBlock->updateContext(new BlockContext());
	testBlock->position = vec3(0.5f * blockSize.x, 0.5f * blockSize.y, 0.5f * blockSize.z);
	testBlock->buildStandalone();

	//Make random more random.
	time_t currentTime;
	time(&currentTime);
	srand((GLuint)currentTime);

	builderThread = new thread([this]() {
		loading = true;
		logger << "Populating world from seed..." << Log::newline;
		world->populateFromSeed(worldSeed);
		logger << "World build" << Log::newline;
		loading = false;
		
		spawnPlayer();

		//world->buildStandalone();
		initPhysics();

		while (true) {
			//world->randomTick();
			world->buildStandalone();
			this_thread::sleep_for(0.01s);
		}
	});

	logger << "Initialized" << Log::newline;

	gameObjects3D.push_back(world);
	gameObjects3D.push_back(player);
	world->addChild(pickHighlight);
	gameObjects3D.push_back(testBlock);
}

//void display()
void VrCraft::draw(const glm::mat4 &projectionMatrix, const glm::mat4 &viewMatrix) {
	//glEnable(GL_CULL_FACE);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	float fac = 50.0f;
	static float lightDirection = 1.5f;
	float lightDistance = worldSize.x * chunkSize.x * blockSize.x * 1.0f;
	lightDirection += 0.001f;

	glm::vec3 lightAngle(cos(lightDirection) * lightDistance, lightDistance, sin(lightDirection) * lightDistance);
	glm::mat4 shadowProjectionMatrix = glm::ortho<float>(-fac, fac, -fac, fac, -5, 250);
	glm::mat4 shadowCameraMatrix = glm::lookAt(lightAngle + glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	{
		shadowMapFbo->bind();
		glDisable(GL_SCISSOR_TEST);
		glViewport(0, 0, shadowMapFbo->getWidth(), shadowMapFbo->getHeight());
		glClearColor(1, 0, 0, 1);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		mat4 modelMatrix = mat4();
		modelMatrix = glm::translate(modelMatrix, vec3(-player->position.x, -player->position.y, -player->position.z));
		Shaders::use(Shaders::FBO_DEPTH);
		Shaders::setProjectionViewMatrix(shadowProjectionMatrix, shadowCameraMatrix);
		for (GameObject* object : gameObjects3D) {
			object->draw(modelMatrix);
		}
		shadowMapFbo->unbind();
	}

	{
		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
		glEnable(GL_SCISSOR_TEST);
		glEnable(GL_BLEND);
		glScissor(viewport[0], viewport[1], viewport[2], viewport[3]);
		glClearColor(0, 0.6f, 1, 1);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadowMapFbo->texid[0]);
		glActiveTexture(GL_TEXTURE0);

		mat4 modelMatrix = mat4();
		modelMatrix = glm::translate(modelMatrix, vec3(-player->position.x, -player->position.y, -player->position.z));
		Shaders::use(randomShader());
		Shaders::setAnimation(lightDirection);
		Shaders::setProjectionViewMatrix(projectionMatrix, viewMatrix);
		Shaders::setShadowMatrix(shadowProjectionMatrix * shadowCameraMatrix);
		for (GameObject* object : gameObjects3D) {
			object->draw(modelMatrix);
		}
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void VrCraft::preFrame(double frameTime, double totalTime) {
	float elapsedSeconds = 0.010f;// (float)(frameTime / 1000.0);

	updateWand(player->primaryHand, primaryWandPosition.getData());

	if (physicsWorld != nullptr) {
		physicsWorld->onUpdate(elapsedSeconds);

		Block* pickedBlock = pickBlock(player->primaryHand);
		if (pickedBlock != nullptr) {
			if (pickHighlight->position != pickedBlock->globalPosition())
				damage = 0;

			pickHighlight->position = pickedBlock->globalPosition();
		}

		physicsWait -= elapsedSeconds;
		if (primaryWandTouch.getData() == vrlib::DigitalState::OFF &&
			primaryWandTrigger.getData() == vrlib::DigitalState::OFF) {
			physicsWait = -1;
			damage = 0;
		}

		if (physicsWait < 0 && primaryWandTouch.getData() == vrlib::DigitalState::ON) {
			physicsWait = 0.25f;
			throwBlock();
		}

		if (primaryWandTrigger.getData() == vrlib::DigitalState::ON) {
			if (pickedBlock != nullptr) {
				damage += elapsedSeconds;
				if (damage >= 1)
					world->setBlock(pickedBlock->globalPosition(), new AirBlock());
			}
		}

		pickHighlight->setDamage(damage);
		pickHighlight->updateContext(new BlockContext());
	}

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
		c->addComponent(new PhysicsComponent(physicsWorld, ShapeType::MESH, true));
	}
}

Block* VrCraft::pickBlock(GameObject* wandObject) {
	Block* pickedBlock = nullptr;
	for (int offset = 0; pickedBlock == nullptr && offset < 5; offset++) {
		pickedBlock = world->getBlock(wandObject->globalPosition() + wandObject->orientation * vec3(0, 0, offset * -1));
		if (dynamic_cast<AirBlock*>(pickedBlock) != nullptr)
			pickedBlock = nullptr; //Cannot pick air.
	}
	return pickedBlock;
}

Shader<Shaders::Uniforms>* VrCraft::randomShader() {
	switch (curShaderIndex) {
	case 0:
		return Shaders::DEFAULT;
	case 1:
		return Shaders::SPECULAR;
	case 2:
		return Shaders::NOISE;
	case 3:
		return Shaders::WAVE;
	case 4:
		return Shaders::TOON;
	case 5:
	default:
		return Shaders::SHADOW;
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
	PhysicsComponent* physComponent = new PhysicsComponent(physicsWorld, ShapeType::BOX, false, newBlock->getBlockSize());
	newBlock->addComponent(new TextureDrawComponent("data/VrCraft/textures/terrain.png"));
	newBlock->addComponent(physComponent);
	newBlock->addComponent(new DespawnComponent(world, 2.0f));
	newBlock->position = player->primaryHand->globalPosition();
	newBlock->orientation = player->orientation;

	newBlock->updateContext(new BlockContext());
	newBlock->buildStandalone();

	physComponent->getBody()->setCollisionListener([this, newBlock](PhysicsRigidBody* other) {
		Chunk* touchedChunk = dynamic_cast<Chunk*>(other->getObject());
		if (touchedChunk != nullptr) {
			BlockContext* context = world->getAdjacentBlocks(newBlock->position);
			if (context->anyAdjacent()) {
				Block* realBlock = new CobblestoneBlock();
				world->setBlock(newBlock->position, realBlock);
				world->deleteChild(newBlock);
			}
		}
	});
	physComponent->getBody()->addForce(player->primaryHand->orientation * vec3(0, 0, -500));

	world->addChild(newBlock);
}

void VrCraft::updateWand(GameObject* wandObject, const mat4& wandMatrix) {
	vec3 axis = vec3((wandMatrix * vec4(0, 0, 1, 1)) - (wandMatrix * vec4(0, 0, 0, 1)));
	wandObject->orientation = quat_cast(wandMatrix);
	wandObject->position = vec3(wandMatrix * vec4(0, 0, 0, 1));
}