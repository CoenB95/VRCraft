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
		shadowMatrix,
		normalMatrix,
		textureSampler,
		shadowSampler,
		diffuseColor,
		textureFactor
	};

public:
	static vrlib::gl::Shader<Uniforms>* DEFAULT_SHADER;
	static vrlib::gl::Shader<Uniforms>* DEPTH;
	static vrlib::gl::Shader<Uniforms>* DEPTH_FBO;
	static vrlib::gl::Shader<Uniforms>* SPECULAR;

	static void setupDefaultShaders();
	static vrlib::gl::Shader<Uniforms>* setupShader(string vertShader, string fragShader);
	static void useShader(vrlib::gl::Shader<Uniforms>* shader, const glm::mat4& projectionMatrix,
		const glm::mat4& viewMatrix, const glm::mat4& modelMatrix, const glm::mat4& shadowMatrix,
		int textureSampler = 0, int shadowSampler = 1);
};
