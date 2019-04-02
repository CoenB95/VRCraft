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

//Outputs (to geomerty/fragment-shader)
out vec2 pixelTextureCoord;
out vec3 pixelNormal;
out vec4 shadowPosition;

mat4 biasMatrix = mat4(
0.5, 0.0, 0.0, 0.0,
0.0, 0.5, 0.0, 0.0,
0.0, 0.0, 0.5, 0.0,
0.5, 0.5, 0.5, 1.0
);

//Shader
void main()
{
	shadowPosition = biasMatrix * shadowMatrix * modelMatrix * vec4(vertexPosition, 1.0);
	
	pixelTextureCoord = vertexTextureCoord;
	pixelNormal = normalMatrix * vertexNormal;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}