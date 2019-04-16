#pragma once

#include <glm/glm.hpp>
#include <mutex>
#include <thread>
#include <VrLib/Application.h>
#include <VrLib/Device.h>
#include <VrLib/gl/FBO.h>

#include "shaders.h"
#include "VrLib/gl/shader.h"

using namespace glm;
using namespace std;

class Block;
class DamageBlock;
class GameObject;
class PhysicsWorld;
class Player;
class Shaders;
class World;

class VrCraft : public vrlib::Application
{
private:
	vec3 worldSize = vec3(4, 4, 4);
	vec3 chunkSize = vec3(16, 16, 16);
	vec3 blockSize = vec3(1, 1, 1);
	vec3 spawnOffset = vec3(0.5f * blockSize.x, 0, 0.5f * blockSize.z);
	int worldSeed = 3;

	vrlib::DigitalDevice primaryWandMenu;
	vrlib::PositionalDevice primaryWandPosition;
	vrlib::DigitalDevice primaryWandTouch;
	vrlib::TwoDimensionDevice primaryWandTouchPosition;
	vrlib::DigitalDevice primaryWandTrigger;
	vrlib::PositionalDevice secondaryWandPosition;
	vrlib::DigitalDevice secondaryWandTouch;
	vrlib::TwoDimensionDevice secondaryWandTouchPosition;
	vrlib::DigitalDevice secondaryWandTrigger;

	World* world = nullptr;
	Player* player = nullptr;
	DamageBlock* pickHighlight = nullptr;

	Block* sun = nullptr;
	float sunDistance = worldSize.x * chunkSize.x * blockSize.x * 1.0f;

	thread* builderThread = nullptr;
	bool loading = true;
	mutex updateMutex;

	float elapsedSecondsFrame = 0;
	float elapsedSecondsTotal = 0;

	PhysicsWorld* physicsWorld = nullptr;
	vrlib::gl::FBO* shadowMapFbo = nullptr;
	vrlib::gl::FBO* renderFbo = nullptr;

public:
	VrCraft();

	virtual void init() override;
	virtual void draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix) override;
	virtual void preFrame(double frameTime, double totalTime) override;

	void initPhysics();
	Block* pickBlock(GameObject* wandObject);
	vrlib::gl::Shader<Shaders::Uniforms>* randomShader();
	void spawnPlayer();
	void throwBlock();
	void throwPearl();
	void updateWand(GameObject* wandObject, const mat4& wandMatrix);
};

