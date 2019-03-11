#pragma once

#include <thread>
#include <VrLib/Application.h>
#include <VrLib/Device.h>

using namespace std;

class VrCraft : public vrlib::Application
{
private:
	vrlib::DigitalDevice secondaryWandInput;
	vrlib::PositionalDevice secondaryWandPosition;
	thread* builderThread;

public:
	VrCraft();

	virtual void init() override;
	virtual void draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix) override;
	virtual void preFrame(double frameTime, double totalTime) override;
};

