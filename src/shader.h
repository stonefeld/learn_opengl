#ifndef __SHADER_H__
#define __SHADER_H__

#include "gfx.h"
#include "utils.h"

typedef struct {
	GLuint handle;
	GLuint vertex_handle;
	GLuint fragment_handle;
} Shader;

Shader shader_create(const char *vertex_path, const char *fragment_path);
void shader_destroy(Shader self);
void shader_bind(Shader self);

// Single values.
void shader_uniform_int(Shader self, const char* name, int v);
void shader_uniform_float(Shader self, const char* name, float v);

// N-dimension vectors.
void shader_uniform_vec2(Shader self, const char* name, vec2 v);
void shader_uniform_float2(Shader self, const char* name, float x, float y);
void shader_uniform_vec3(Shader self, const char* name, vec3 v);
void shader_uniform_float3(Shader self, const char* name, float x, float y, float z);
void shader_uniform_vec4(Shader self, const char* name, vec4 v);
void shader_uniform_float4(Shader self, const char* name, float x, float y, float z, float w);

// Matrices.
void shader_uniform_mat2(Shader self, const char* name, mat2 v);
void shader_uniform_mat3(Shader self, const char* name, mat3 v);
void shader_uniform_mat4(Shader self, const char* name, mat4 v);

#endif // __SHADER_H__
