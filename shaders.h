#pragma once

#include <VrLib/gl/shader.h>
#include <string>

using namespace glm;
using namespace std;
using namespace vrlib::gl;

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

private:
	static Shader<Uniforms>* activeShader;

public:
	static vrlib::gl::Shader<Uniforms>* DEFAULT_SHADER;
	static vrlib::gl::Shader<Uniforms>* DEPTH;
	static vrlib::gl::Shader<Uniforms>* DEPTH_FBO;
	static vrlib::gl::Shader<Uniforms>* SPECULAR;

	static void setupDefaultShaders();

	static void setProjectionViewMatrix(const mat4& projectionMatrix, const mat4& viewMatrix);
	static void setModelMatrix(const mat4& modelMatrix);
	static void setShadowMatrix(const mat4& shadowMatrix);

	static vrlib::gl::Shader<Uniforms>* setup(string vertShader, string fragShader);
	static void use(Shader<Uniforms>* shader);
};
