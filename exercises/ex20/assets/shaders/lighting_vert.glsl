#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 LightingColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	vec3 position = vec3(model * vec4(aPos, 1.0));
	vec3 normal = mat3(transpose(inverse(model))) * aNormal;

	// ambient light.
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	// diffuse light.
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// specular light.
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - position);
	vec3 reflectDir = reflect(-lightDir, norm); // negate lightDir since its pointing from the fragment to the light source and not otherwise.
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	// final color
	LightingColor = ambient + diffuse + specular;
}
