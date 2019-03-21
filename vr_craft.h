#pragma once

#include <glm/glm.hpp>
#include <mutex>
#include <thread>
#include <VrLib/Application.h>
#include <VrLib/Device.h>

using namespace glm;
using namespace std;

class PhysicsWorld;

class VrCraft : public vrlib::Application
{
private:
	vrlib::DigitalDevice secondaryWandInput;
	vrlib::PositionalDevice secondaryWandPosition;
	thread* builderThread;
	bool loading = true;
	mutex updateMutex;

	vec3 worldSize = vec3(4, 4, 4);
	vec3 chunkSize = vec3(16, 16, 16);
	vec3 blockSize = vec3(1, 1, 1);
	int worldSeed = 3;

	PhysicsWorld* physicsWorld;

public:
	VrCraft();

	virtual void init() override;
	virtual void draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix) override;
	virtual void preFrame(double frameTime, double totalTime) override;

	void initPhysics();
	void spawnPlayer();
};

