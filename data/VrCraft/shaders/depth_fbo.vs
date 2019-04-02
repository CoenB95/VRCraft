#version 330

//Uniforms
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;

//Arguments (vertices)
in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTextureCoord;

//Outputs (to geometry/fragment-shader)

//Shader
void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1);
}