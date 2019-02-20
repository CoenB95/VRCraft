#include "shaders.h"

vrlib::gl::Shader<Shaders::Uniforms>* Shaders::DEFAULT_SHADER = nullptr;

void Shaders::setupDefaultShaders() {
	DEFAULT_SHADER = setupShader("data/VrCraft/shaders/default.vert", "data/VrCraft/shaders/default.frag");
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
	shader->registerUniform(Uniforms::s_texture, "s_texture");
	shader->registerUniform(Uniforms::diffuseColor, "diffuseColor");
	shader->registerUniform(Uniforms::textureFactor, "textureFactor");
	shader->use();
	shader->setUniform(Uniforms::s_texture, 0);
	return shader;
}

void Shaders::useShader(vrlib::gl::Shader<Shaders::Uniforms>* shader, const glm::mat4& projectionMatrix, const glm::mat4& modelViewMatrix) {
	shader->use();
	shader->setUniform(Shaders::Uniforms::projectionMatrix, projectionMatrix);
	shader->setUniform(Shaders::Uniforms::viewMatrix, modelViewMatrix);
	shader->setUniform(Shaders::Uniforms::modelMatrix, glm::mat4());
	shader->setUniform(Shaders::Uniforms::diffuseColor, glm::vec4(1, 1, 1, 1));
	shader->setUniform(Shaders::Uniforms::textureFactor, 1.0f);
}