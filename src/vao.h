#ifndef VAO_H
#define VAO_H

#include "gfx.h"
#include "utils.h"
#include "vbo.h"

struct opengl_vao
{
	GLuint handle;
};

struct opengl_vao vao_create();
void vao_destroy(struct opengl_vao self);

void vao_bind(struct opengl_vao self);
void vao_unbind(void);
void vao_attr(struct opengl_vao self, struct opengl_vbo vbo, GLuint index, GLint size, GLenum type, GLsizei stride, size_t offset);

#endif
