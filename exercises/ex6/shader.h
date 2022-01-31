#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include "types.h"
#include "utils.h"

typedef struct
{
	GLuint handle;
	GLuint vertex_handle;
	GLuint fragment_handle;
} shader;

shader shader_create(const char* vertex_path, const char* fragment_path);
void shader_destroy(shader self);
void shader_bind(shader self);

void shader_uniform_1i(shader self, const char* name, vec1i v);
void shader_uniform_2i(shader self, const char* name, vec2i v);
void shader_uniform_3i(shader self, const char* name, vec3i v);
void shader_uniform_4i(shader self, const char* name, vec4i v);

void shader_uniform_1f(shader self, const char* name, vec1f v);
void shader_uniform_2f(shader self, const char* name, vec2f v);
void shader_uniform_3f(shader self, const char* name, vec3f v);
void shader_uniform_4f(shader self, const char* name, vec4f v);

#endif
