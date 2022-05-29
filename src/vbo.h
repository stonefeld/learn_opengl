#ifndef VBO_H
#define VBO_H

#include "gfx.h"
#include "utils.h"

typedef struct {
	GLuint handle;
	GLint type;
	bool dynamic;
} Vbo;

Vbo vbo_create(GLint type, bool dynamic);
void vbo_destroy(Vbo self);

void vbo_bind(Vbo self);
void vbo_unbind(Vbo self);
void vbo_buffer(Vbo self, GLsizeiptr size, void* data);

#endif
