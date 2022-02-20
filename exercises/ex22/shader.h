#ifndef SHADER_H
#define SHADER_H

#include "gfx.h"
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

// Single values.
void shader_uniform_int(struct opengl_shader self, const char* name, int v);
void shader_uniform_float(struct opengl_shader self, const char* name, float v);

// N-dimension vectors.
void shader_unifrom_vec2(struct opengl_shader self, const char* name, vec2 v);
void shader_uniform_float2(struct opengl_shader self, const char* name, float x, float y);
void shader_uniform_vec3(struct opengl_shader self, const char* name, vec3 v);
void shader_uniform_float3(struct opengl_shader self, const char* name, float x, float y, float z);
void shader_uniform_vec4(struct opengl_shader self, const char* name, vec4 v);
void shader_uniform_float4(struct opengl_shader self, const char* name, float x, float y, float z, float w);

// Matrices.
void shader_uniform_mat2(struct opengl_shader self, const char* name, mat2 v);
void shader_uniform_mat3(struct opengl_shader self, const char* name, mat3 v);
void shader_uniform_mat4(struct opengl_shader self, const char* name, mat4 v);

#endif
