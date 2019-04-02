//Basic environment lighting fragment shader.
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
	vec3 lightDirection = normalize(vec3(1, 1, 1));
	vec3 viewDirection = vec3(0, 0, 1);
	float shininess = 100.0;

	float ambient = 0.7;
	float diffuse = 0.8 * dot(normalize(pixelNormal), lightDirection);

	vec3 r = reflect(-lightDirection, normalize(pixelNormal));

	float specular = pow(max(0.0, dot(r, viewDirection)), shininess);

	float factor = ambient + diffuse + specular;

	vec4 textureColor = texture2D(uniTextureSampler, pixelTextureCoord);
	if(textureColor.a < 0.01)
		discard;
	
	fragColor.rgb = factor * textureColor.rgb;
	fragColor.a = textureColor.a;
}