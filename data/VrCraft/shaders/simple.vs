#version 330

in vec3 a_position;
in vec3 a_normal;
in vec2 a_texcoord;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;

out vec3 normal;

void main()
{
	normal = normalMatrix * a_normal;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(a_position,1);
}