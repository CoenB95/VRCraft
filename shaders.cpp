#include <iostream>
#include <VrLib/Log.h>

#include "shaders.h"

using vrlib::Log;
using vrlib::logger;

vrlib::gl::Shader<Shaders::Uniforms>* Shaders::DEFAULT_SHADER = nullptr;
vrlib::gl::Shader<Shaders::Uniforms>* Shaders::DEPTH = nullptr;
vrlib::gl::Shader<Shaders::Uniforms>* Shaders::DEPTH_FBO = nullptr;
vrlib::gl::Shader<Shaders::Uniforms>* Shaders::SPECULAR = nullptr;

void Shaders::setupDefaultShaders() {
	DEFAULT_SHADER = setupShader("data/VrCraft/shaders/default.vs", "data/VrCraft/shaders/default.fs");
	DEPTH = setupShader("data/VrCraft/shaders/depth.vs", "data/VrCraft/shaders/depth.fs");
	DEPTH_FBO = setupShader("data/VrCraft/shaders/depth_fbo.vs", "data/VrCraft/shaders/depth_fbo.fs");
	SPECULAR = setupShader("data/VrCraft/shaders/simple.vs", "data/VrCraft/shaders/simple.fs");
}

vrlib::gl::Shader<Shaders::Uniforms>* Shaders::setupShader(string vertShader, string fragShader) {
	vrlib::gl::Shader<Shaders::Uniforms>* shader = new vrlib::gl::Shader<Uniforms>(vertShader, fragShader);
	shader->bindAttributeLocation("vertexPosition", 0);
	shader->bindAttributeLocation("vertexNormal", 1);
	shader->bindAttributeLocation("vertexTextureCoord", 2);
	shader->link();
	shader->bindFragLocation("fragColor", 0);
	shader->registerUniform(Uniforms::modelMatrix, "modelMatrix");
	shader->registerUniform(Uniforms::projectionMatrix, "projectionMatrix");
	shader->registerUniform(Uniforms::viewMatrix, "viewMatrix");
	shader->registerUniform(Uniforms::normalMatrix, "normalMatrix");
	shader->registerUniform(Uniforms::textureSampler, "textureSampler");
	shader->registerUniform(Uniforms::shadowSampler, "shadowSampler");
	shader->registerUniform(Uniforms::diffuseColor, "diffuseColor");
	shader->registerUniform(Uniforms::textureFactor, "textureFactor");
	return shader;
}

void Shaders::useShader(vrlib::gl::Shader<Shaders::Uniforms>* shader, const glm::mat4& projectionMatrix,
	const glm::mat4& viewMatrix, const glm::mat4& modelMatrix, int textureSampler) {
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));

	shader->use();
	shader->setUniform(Uniforms::projectionMatrix, projectionMatrix);
	shader->setUniform(Uniforms::viewMatrix, viewMatrix);
	shader->setUniform(Uniforms::modelMatrix, modelMatrix);
	shader->setUniform(Uniforms::normalMatrix, normalMatrix);
	shader->setUniform(Uniforms::textureSampler, textureSampler);
	shader->setUniform(Uniforms::diffuseColor, glm::vec4(1, 1, 1, 1));
	shader->setUniform(Uniforms::textureFactor, 1.0f);
}
