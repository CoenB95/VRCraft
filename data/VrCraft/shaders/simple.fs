#version 330

uniform sampler2D s_texture;
uniform vec4 diffuseColor;
uniform float textureFactor;

in vec3 normal;
in vec2 texCoord;

out vec4 fragColor;

void main()
{
	vec3 lightDirection = normalize(vec3(1,1,1));
	vec3 viewDirection = vec3(0,0,1);
	float shininess = 100.0;

	float ambient = 0.2;
	float diffuse = 0.8 * clamp(dot(normalize(normal), lightDirection), 0, 1);

	vec3 r = reflect(-lightDirection, normalize(normal));

	float specular = pow(max(0.0, dot(r, viewDirection)), shininess);

	float factor = ambient + diffuse + specular;


	vec4 tex = mix(diffuseColor, texture2D(s_texture, texCoord), textureFactor);
	if(tex.a < 0.01)
		discard;
	fragColor.rgb = factor * tex.rgb;
	fragColor.a = tex.a;
}