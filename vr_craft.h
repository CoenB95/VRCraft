#pragma once

#include <VrLib/Application.h>
#include <VrLib/Device.h>

class VrCraft : public vrlib::Application
{
private:
	vrlib::DigitalDevice leftButton;

public:
	VrCraft();

	virtual void init() override;
	virtual void draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix) override;
	virtual void preFrame(double frameTime, double totalTime) override;
};

