#include <iostream>
#include <VrLib/Log.h>

#include "shaders.h"

using vrlib::Log;
using vrlib::logger;

using namespace glm;
using namespace vrlib::gl;

Shader<Shaders::Uniforms>* Shaders::activeShader = nullptr;

Shader<Shaders::Uniforms>* Shaders::DEFAULT = nullptr;
Shader<Shaders::Uniforms>* Shaders::FBO_DEPTH = nullptr;
Shader<Shaders::Uniforms>* Shaders::NOISE = nullptr;
Shader<Shaders::Uniforms>* Shaders::SHADOW = nullptr;
Shader<Shaders::Uniforms>* Shaders::SPECULAR = nullptr;
Shader<Shaders::Uniforms>* Shaders::TOON = nullptr;
Shader<Shaders::Uniforms>* Shaders::WAVE = nullptr;

void Shaders::setupDefaultShaders() {
	DEFAULT = setup("data/VrCraft/shaders/default.vs", "data/VrCraft/shaders/default.fs");
	FBO_DEPTH = setup("data/VrCraft/shaders/depth_fbo.vs", "data/VrCraft/shaders/depth_fbo.fs");
	NOISE = setup("data/VrCraft/shaders/default.vs", "data/VrCraft/shaders/noise_anim.fs");
	SHADOW = setup("data/VrCraft/shaders/depth.vs", "data/VrCraft/shaders/depth.fs");
	SPECULAR = setup("data/VrCraft/shaders/default.vs", "data/VrCraft/shaders/specular.fs");
	TOON = setup("data/VrCraft/shaders/default.vs", "data/VrCraft/shaders/toon.fs");
	WAVE = setup("data/VrCraft/shaders/wave.vs", "data/VrCraft/shaders/default.fs");
	activeShader = DEFAULT;
}

vrlib::gl::Shader<Shaders::Uniforms>* Shaders::setup(string vertShader, string fragShader) {
	Shader<Shaders::Uniforms>* shader = new Shader<Uniforms>(vertShader, fragShader);
	shader->bindAttributeLocation("vertexPosition", 0);
	shader->bindAttributeLocation("vertexNormal", 1);
	shader->bindAttributeLocation("vertexTextureCoord", 2);
	shader->link();
	shader->bindFragLocation("fragColor", 0);
	shader->registerUniform(Uniforms::modelMatrix, "modelMatrix");
	shader->registerUniform(Uniforms::projectionMatrix, "projectionMatrix");
	shader->registerUniform(Uniforms::viewMatrix, "viewMatrix");
	shader->registerUniform(Uniforms::shadowMatrix, "shadowMatrix");
	shader->registerUniform(Uniforms::normalMatrix, "normalMatrix");
	shader->registerUniform(Uniforms::textureSampler, "textureSampler");
	shader->registerUniform(Uniforms::shadowSampler, "shadowSampler");
	shader->registerUniform(Uniforms::diffuseColor, "diffuseColor");
	shader->registerUniform(Uniforms::textureFactor, "textureFactor");
	shader->registerUniform(Uniforms::animTime, "animTime");
	return shader;
}

void Shaders::use(Shader<Shaders::Uniforms>* shader) {
	activeShader = shader;
	activeShader->use();
	activeShader->setUniform(Uniforms::diffuseColor, vec4(1, 1, 1, 1));
	activeShader->setUniform(Uniforms::textureFactor, 1.0f);
	activeShader->setUniform(Uniforms::textureSampler, 0);
	activeShader->setUniform(Uniforms::shadowSampler, 1);
}

void Shaders::setAnimation(float time) {
	activeShader->setUniform(Uniforms::animTime, time);
}

void Shaders::setModelMatrix(const mat4& modelMatrix) {
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	activeShader->setUniform(Uniforms::modelMatrix, modelMatrix);
	activeShader->setUniform(Uniforms::normalMatrix, normalMatrix);
}

void Shaders::setProjectionViewMatrix(const mat4& projectionMatrix, const mat4& viewMatrix) {
	activeShader->setUniform(Uniforms::projectionMatrix, projectionMatrix);
	activeShader->setUniform(Uniforms::viewMatrix, viewMatrix);
}

void Shaders::setShadowMatrix(const mat4& shadowMatrix) {
	activeShader->setUniform(Uniforms::shadowMatrix, shadowMatrix);
}