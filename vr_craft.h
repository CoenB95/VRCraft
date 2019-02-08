#pragma once

#include <VrLib/Application.h>
#include <VrLib/Device.h>
#include <VrLib/gl/shader.h>

#include <list>


class VrCraft : public vrlib::Application
{
	enum class Uniforms
	{
		modelMatrix,
		projectionMatrix,
		viewMatrix,
		s_texture,
		diffuseColor,
		textureFactor
	};

private:
	vrlib::gl::Shader<Uniforms>* shader;
	vrlib::DigitalDevice leftButton;

public:
	VrCraft();

	virtual void init() override;
	virtual void draw(const glm::mat4 &projectionMatrix, const glm::mat4 &modelViewMatrix) override;
	virtual void preFrame(double frameTime, double totalTime) override;
};

