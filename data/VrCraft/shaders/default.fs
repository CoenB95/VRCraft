//Basic texture fragment shader.
#version 330

//Uniforms
uniform sampler2D uniTextureSampler;
uniform sampler2DShadow uniShadowSampler;
uniform vec4 uniDiffuseColor;
uniform float uniTextureFactor;

//Inputs (from geometry/vertex-shader)
in vec2 pixelTextureCoord;
in vec3 pixelNormal;

//Outputs (to draw buffers)
out vec4 fragColor;

//Shader
void main()
{
	vec4 textureColor = texture(uniTextureSampler, pixelTextureCoord);
	
	fragColor = textureColor;
}
