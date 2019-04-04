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
		textureFactor,
		animTime
	};

private:
	static Shader<Uniforms>* activeShader;

public:
	static Shader<Uniforms>* DEFAULT;
	static Shader<Uniforms>* FBO_DEPTH;
	static Shader<Uniforms>* NOISE;
	static Shader<Uniforms>* SHADOW;
	static Shader<Uniforms>* SPECULAR;
	static Shader<Uniforms>* TOON;
	static Shader<Uniforms>* WAVE;

	static void setupDefaultShaders();

	static void setAnimation(float time);
	static void setProjectionViewMatrix(const mat4& projectionMatrix, const mat4& viewMatrix);
	static void setModelMatrix(const mat4& modelMatrix);
	static void setShadowMatrix(const mat4& shadowMatrix);

	static Shader<Uniforms>* setup(string vertShader, string fragShader);
	static void use(Shader<Uniforms>* shader);
};
