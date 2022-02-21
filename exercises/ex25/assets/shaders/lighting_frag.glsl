#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	// ambient light.
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	// diffuse light.
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse * texture(material.diffuse, TexCoords).rgb;

	// specular light.
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // negate lightDir since its pointing from the fragment to the light source and not otherwise.
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * light.specular * texture(material.specular, TexCoords).rbg;

	// emission light.
	vec3 emission = texture(material.emission, TexCoords).rgb;

	// final color
	vec3 result = ambient + diffuse + specular + emission;
	FragColor = vec4(result, 1.0);
}
