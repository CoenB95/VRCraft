#version 330

//Uniforms
uniform sampler2D textureSampler;
uniform sampler2DShadow shadowSampler;
uniform vec4 diffuseColor;
uniform float textureFactor;

//Input arguments (from geometry/vertex-shader)
in vec2 pixelTextureCoord;
in vec3 pixelNormal;
in vec4 shadowPosition;

//Outputs (to draw buffers)
out vec4 fragColor;

//Shader
void main()
{
	float diffuse = dot(normalize(vec3(1, 1, 1)), normalize(normal));
	diffuse = clamp(diffuse, 0, 1);

	float bias = 0.001;
	float visibility = 1.0;
	for (int i = 0; i < 4; i++) {
		int index = i;
		visibility -= 0.15 * (1.0 - texture(uniShadowSampler, vec3(shadowPosition.xy, (shadowPosition.z-bias) / shadowPosition.w)));
	}
	float light = 0.5 * diffuse + 0.5;
	vec4 tex = mix(diffuseColor, texture2D(s_texture, texCoord), textureFactor);
	if(tex.a < 0.01)
		discard;
	fragColor.rgb = visibility * light * tex.rgb;
	fragColor.a = tex.a;
}