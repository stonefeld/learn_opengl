#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position; // no longer needed when using direction lights.
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float cutOffOuter;
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
	// vec3 lightDir = normalize(-light.direction);
	vec3 lightDir = normalize(light.position - FragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.cutOffOuter;
	float intensity = clamp((theta - light.cutOffOuter) / epsilon, 0.0, 1.0);

	// ambient light
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	// diffuse light
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * light.diffuse * texture(material.diffuse, TexCoords).rgb;

	// specular light
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // negate lightDir since its pointing from the fragment to the light source and not otherwise.
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = spec * light.specular * texture(material.specular, TexCoords).rgb;

	// calculate attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// adapt light with the attenuation
	// ambient *= attenuation; // remove attenuation from ambient, as otherwise at large distances would be darker inside than outside.
	diffuse *= attenuation;
	specular *= attenuation;

	// now adapt the light with the intensity
	diffuse *= intensity;
	specular *= intensity;

	// final color
	FragColor = vec4(ambient + diffuse + specular, 1.0);
}
