#ifndef VAO_H
#define VAO_H

#include "gfx.h"
#include "utils.h"
#include "vbo.h"

typedef struct {
	GLuint handle;
} Vao;

Vao vao_create();
void vao_destroy(Vao self);

void vao_bind(Vao self);
void vao_unbind(void);
void vao_attr(Vao self, Vbo vbo, GLuint index, GLint size, GLenum type, GLsizei stride, size_t offset);

#endif
