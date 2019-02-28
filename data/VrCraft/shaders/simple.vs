#version 330

in vec3 a_position;
in vec3 a_normal;
in vec2 a_texture;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

out vec3 normal;
out vec2 texCoord;

void main()
{
	texCoord = a_texture;
	normal = normalMatrix * a_normal;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position, 1.0);
}