#version 330 core

layout (location = 0) in vec3 i_vertpos;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec2 i_texcoords;

out vec3 t_fragpos;
out vec3 t_normal;
out vec2 t_texcoords;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
	t_fragpos = vec3(u_model * vec4(i_vertpos, 1.0));
	t_normal = mat3(transpose(inverse(u_model))) * i_normal;
	t_texcoords = i_texcoords;

	gl_Position = u_projection * u_view * u_model * vec4(i_vertpos, 1.0);
}
