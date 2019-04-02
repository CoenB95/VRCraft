//Basic texture fragment shader.
#version 330

//Uniforms
uniform sampler2D textureSampler;
uniform sampler2DShadow shadowSampler;
uniform vec4 diffuseColor;
uniform float textureFactor;

//Inputs (from geometry/vertex-shader)
in vec2 pixelTextureCoord;
in vec3 pixelNormal;

//Outputs (to draw buffers)
out vec4 fragColor;

//Shader
void main()
{
	vec4 textureColor = texture(textureSampler, pixelTextureCoord);
	
	fragColor = textureColor;
}
