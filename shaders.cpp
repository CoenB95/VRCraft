#include <iostream>
#include <VrLib/Log.h>

#include "shaders.h"

using vrlib::Log;
using vrlib::logger;

vrlib::gl::Shader<Shaders::Uniforms>* Shaders::DEFAULT_SHADER = nullptr;
vrlib::gl::Shader<Shaders::Uniforms>* Shaders::SPECULAR = nullptr;

void Shaders::setupDefaultShaders() {
	logger << "Def shad" << Log::newline;
	DEFAULT_SHADER = setupShader("data/VrCraft/shaders/default.vert", "data/VrCraft/shaders/default.frag");
	logger << "Spec shad" << Log::newline;
	SPECULAR = setupShader("data/VrCraft/shaders/simple.vs", "data/VrCraft/shaders/simple.fs");
}

vrlib::gl::Shader<Shaders::Uniforms>* Shaders::setupShader(string vertShader, string fragShader) {
	vrlib::gl::Shader<Shaders::Uniforms>* shader = new vrlib::gl::Shader<Uniforms>(vertShader, fragShader);
	shader->bindAttributeLocation("a_position", 0);
	shader->bindAttributeLocation("a_normal", 1);
	shader->bindAttributeLocation("a_texcoord", 2);
	shader->link();
	shader->bindFragLocation("fragColor", 0);
	shader->registerUniform(Uniforms::modelMatrix, "modelMatrix");
	shader->registerUniform(Uniforms::projectionMatrix, "projectionMatrix");
	shader->registerUniform(Uniforms::viewMatrix, "viewMatrix");
	shader->registerUniform(Uniforms::normalMatrix, "normalMatrix");
	shader->registerUniform(Uniforms::s_texture, "s_texture");
	shader->registerUniform(Uniforms::diffuseColor, "diffuseColor");
	shader->registerUniform(Uniforms::textureFactor, "textureFactor");
	shader->use();
	shader->setUniform(Uniforms::s_texture, 0);
	return shader;
}

void Shaders::useShader(vrlib::gl::Shader<Shaders::Uniforms>* shader, const glm::mat4& projectionMatrix,
	const glm::mat4& viewMatrix, const glm::mat4& modelMatrix) {
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(/*viewMatrix **/ modelMatrix)));

	shader->use();
	shader->setUniform(Uniforms::projectionMatrix, projectionMatrix);
	shader->setUniform(Uniforms::viewMatrix, viewMatrix);
	shader->setUniform(Uniforms::modelMatrix, modelMatrix);
	shader->setUniform(Uniforms::normalMatrix, normalMatrix);
	shader->setUniform(Uniforms::diffuseColor, glm::vec4(1, 1, 1, 1));
	shader->setUniform(Uniforms::textureFactor, 1.0f);
}
