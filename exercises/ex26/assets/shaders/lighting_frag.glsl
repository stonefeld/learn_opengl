#version 330 core

#define NR_POINT_LIGHTS 4

struct glsl_material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct glsl_dirlight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct glsl_pointlight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct glsl_spotlight
{
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutoff;
	float cutoff_outer;
};

in vec3 t_fragpos;
in vec3 t_normal;
in vec2 t_texcoords;

uniform vec3 u_viewpos;
uniform glsl_material u_material;
uniform glsl_dirlight u_dirlight;
uniform glsl_pointlight u_pointlights[NR_POINT_LIGHTS];
uniform glsl_spotlight u_spotlight;

vec3 calculate_dirlight(glsl_dirlight light, vec3 normal, vec3 viewdir);
vec3 calculate_pointlight(glsl_pointlight light, vec3 normal, vec3 fragpos, vec3 viewdir);
vec3 calculate_spotlight(glsl_spotlight light, vec3 normal, vec3 fragpos, vec3 viewdir);

vec3
calculate_dirlight(glsl_dirlight light, vec3 normal, vec3 viewdir)
{
	vec3 lightdir = normalize(-light.direction);

	// diffuse shading
	float diff = max(dot(normal, lightdir), 0.0);

	// specular shading
	vec3 reflectdir = reflect(-lightdir, normal);
	float spec = pow(max(dot(viewdir, reflectdir), 0.0), u_material.shininess);

	// combine results
	vec3 ambient = light.ambient * texture(u_material.diffuse, t_texcoords).rgb;
	vec3 diffuse = light.diffuse * diff * texture(u_material.diffuse, t_texcoords).rgb;
	vec3 specular = light.specular * spec * texture(u_material.specular, t_texcoords).rgb;

	// return value
	return(ambient + diffuse + specular);
}

vec3
calculate_pointlight(glsl_pointlight light, vec3 normal, vec3 fragpos, vec3 viewdir)
{
	vec3 lightdir = normalize(light.position - fragpos);

	// diffuse shading
	float diff = max(dot(normal, lightdir), 0.0);

	// specular shading
	vec3 reflectdir = reflect(-lightdir, normal);
	float spec = pow(max(dot(viewdir, reflectdir), 0.0), u_material.shininess);

	// attenuation
	float distance = length(light.position - fragpos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// combine results
	vec3 ambient = light.ambient * texture(u_material.diffuse, t_texcoords).rgb;
	vec3 diffuse = light.diffuse * diff * texture(u_material.diffuse, t_texcoords).rgb;
	vec3 specular = light.specular * spec * texture(u_material.specular, t_texcoords).rgb;

	// apply attenuation
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// return value
	return(ambient + diffuse + specular);
}

vec3
calculate_spotlight(glsl_spotlight light, vec3 normal, vec3 fragpos, vec3 viewdir)
{
	vec3 lightdir = normalize(light.position - fragpos);

	// diffuse shading
	float diff = max(dot(normal, lightdir), 0.0);

	// specular shading
	vec3 reflectdir = reflect(-lightdir, normal);
	float spec = pow(max(dot(viewdir, reflectdir), 0.0), u_material.shininess);

	// attenuation
	float distance = length(light.position - fragpos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// intensity
	float theta = dot(lightdir, normalize(-light.direction));
	float epsilon = light.cutoff - light.cutoff_outer;
	float intensity = clamp((theta - light.cutoff_outer) / epsilon, 0.0, 1.0);

	// combine results
	vec3 ambient = light.ambient * texture(u_material.diffuse, t_texcoords).rgb;
	vec3 diffuse = light.diffuse * diff * texture(u_material.diffuse, t_texcoords).rgb;
	vec3 specular = light.specular * spec * texture(u_material.specular, t_texcoords).rgb;

	// apply attenuation
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// apply intensity
	diffuse *= intensity;
	specular *= intensity;

	// return value
	return(ambient + diffuse + specular);
}

void
main()
{
	// properties
	vec3 norm = normalize(t_normal);
	vec3 viewdir = normalize(u_viewpos - t_fragpos);

	// directional light
	vec3 result = calculate_dirlight(u_dirlight, norm, viewdir);

	// point lights
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += calculate_pointlight(u_pointlights[i], norm, t_fragpos, viewdir);
	}

	// spot light
	result += calculate_spotlight(u_spotlight, norm, t_fragpos, viewdir);

	// final color
	gl_FragColor = vec4(result, 1.0);
}
