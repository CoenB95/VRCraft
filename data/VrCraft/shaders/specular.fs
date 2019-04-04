//Basic environment lighting fragment shader.
#version 330

//Uniforms
uniform sampler2D textureSampler;
uniform sampler2DShadow shadowSampler;
uniform vec4 diffuseColor;
uniform float textureFactor;
uniform float animTime;

//Inputs (from geometry/vertex-shader)
in vec2 pixelTextureCoord;
in vec3 pixelNormal;

//Outputs (to draw buffers)
out vec4 fragColor;

//Shader
void main()
{
	vec3 lightDirection = normalize(vec3(sin(animTime), 1, cos(animTime)));
	vec3 viewDirection = normalize(vec3(0, 1, 1));
	
	float shininess = 10.0;

	float ambient = 0.2;
	float diffuse = clamp(dot(normalize(pixelNormal), lightDirection), 0, 1);

	vec3 r = reflect(-lightDirection, normalize(pixelNormal));
	float specular = pow(max(0.0, dot(r, viewDirection)), shininess);

	float lighting = clamp(ambient + 0.8 * diffuse + specular, 0, 1);

	vec4 textureColor = texture(textureSampler, pixelTextureCoord);
	if(textureColor.a < 0.01)
		discard;
	
	fragColor.rgb = lighting * textureColor.rgb;
	fragColor.a = textureColor.a;
}