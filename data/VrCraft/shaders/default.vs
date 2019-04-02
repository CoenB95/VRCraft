//Basic vertex shader.
#version 330

//Uniforms
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 shadowMatrix;
uniform mat3 normalMatrix;

//Inputs (vertices)
in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTextureCoord;

//Outputs (to geometry/fragment-shader)
out vec2 pixelTextureCoord;
out vec3 pixelNormal;

//Shader
void main()
{
	pixelTextureCoord = vertexTextureCoord;
	pixelNormal = normalMatrix * vertexNormal;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}