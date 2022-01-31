#ifndef VBO_H
#define VBO_H

#include "gfx.h"
#include "utils.h"

struct opengl_vbo
{
	GLuint handle;
	GLint type;
	bool dynamic;
};

struct opengl_vbo vbo_create(GLint type, bool dynamic);
void vbo_destroy(struct opengl_vbo self);

void vbo_bind(struct opengl_vbo self);
void vbo_unbind(struct opengl_vbo self);
void vbo_buffer(struct opengl_vbo self, GLsizeiptr size, void* data);

#endif
