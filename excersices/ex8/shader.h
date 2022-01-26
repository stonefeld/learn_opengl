#ifndef SHADER_H
#define SHADER_H

#include "gfx.h"
#include "types.h"
#include "utils.h"

struct opengl_shader
{
	GLuint handle;
	GLuint vertex_handle;
	GLuint fragment_handle;
};

struct opengl_shader shader_create(const char* vertex_path, const char* fragment_path);
void shader_destroy(struct opengl_shader self);
void shader_bind(struct opengl_shader self);

void shader_uniform_1i(struct opengl_shader self, const char* name, int v);
void shader_uniform_2i(struct opengl_shader self, const char* name, vec2i v);
void shader_uniform_3i(struct opengl_shader self, const char* name, vec3i v);
void shader_uniform_4i(struct opengl_shader self, const char* name, vec4i v);

void shader_uniform_1f(struct opengl_shader self, const char* name, float v);
void shader_uniform_2f(struct opengl_shader self, const char* name, vec2f v);
void shader_uniform_3f(struct opengl_shader self, const char* name, vec3f v);
void shader_uniform_4f(struct opengl_shader self, const char* name, vec4f v);

#endif
