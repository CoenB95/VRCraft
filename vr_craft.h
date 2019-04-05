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
class GameObject;
class PhysicsWorld;
class Shaders;

class VrCraft : public vrlib::Application
{
private:
	vrlib::DigitalDevice primaryWandMenu;
	vrlib::PositionalDevice primaryWandPosition;
	vrlib::DigitalDevice primaryWandTouch;
	vrlib::TwoDimensionDevice primaryWandTouchPosition;
	vrlib::DigitalDevice primaryWandTrigger;
	vrlib::PositionalDevice secondaryWandPosition;
	vrlib::DigitalDevice secondaryWandTouch;
	vrlib::TwoDimensionDevice secondaryWandTouchPosition;
	vrlib::DigitalDevice secondaryWandTrigger;

	Block* primaryWand;

	thread* builderThread;
	bool loading = true;
	mutex updateMutex;

	vec3 worldSize = vec3(4, 4, 4);
	vec3 chunkSize = vec3(16, 16, 16);
	vec3 blockSize = vec3(1, 1, 1);
	int worldSeed = 3;

	PhysicsWorld* physicsWorld;
	vrlib::gl::FBO* shadowMapFbo;

public:
	VrCraft();

	virtual void init() override;
	virtual void draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix) override;
	virtual void preFrame(double frameTime, double totalTime) override;

	void destroyBlock();
	void initPhysics();
	vrlib::gl::Shader<Shaders::Uniforms>* randomShader();
	void spawnPlayer();
	void throwBlock();
	void updateWand(GameObject* wandObject, const mat4& wandMatrix);
};

