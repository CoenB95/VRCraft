#pragma once

#include <VrLib/gl/shader.h>
#include <string>

using namespace std;

class Shaders {
public:
	enum class Uniforms
	{
		modelMatrix,
		projectionMatrix,
		viewMatrix,
		s_texture,
		diffuseColor,
		textureFactor
	};

public:
	static vrlib::gl::Shader<Uniforms>* DEFAULT_SHADER;

	static void setupDefaultShaders();
	static vrlib::gl::Shader<Uniforms>* setupShader(string vertShader, string fragShader);
	static void useShader(vrlib::gl::Shader<Uniforms>* shader, const glm::mat4& projectionMatrix, const glm::mat4& modelViewMatrix);
};
