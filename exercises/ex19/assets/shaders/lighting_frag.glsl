#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	// ambient light.
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse light.
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular light.
	float specularStrength = 0.5;
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm); // negate lightDir since its pointing from the fragment to the light source and not otherwise.
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	// final color
	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}
