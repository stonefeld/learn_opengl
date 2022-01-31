#include "vbo.h"

struct opengl_vbo
vbo_create(GLint type, bool dynamic)
{
	struct opengl_vbo self = {
		.type = type,
		.dynamic = dynamic
	};
	glGenBuffers(1, &self.handle);
	return(self);
}

void
vbo_destroy(struct opengl_vbo self)
{
	glDeleteBuffers(1, &self.handle);
}

void
vbo_bind(struct opengl_vbo self)
{
	glBindBuffer(self.type, self.handle);
}

void
vbo_unbind(struct opengl_vbo self)
{
	glBindBuffer(self.type, 0);
}

void
vbo_buffer(struct opengl_vbo self, GLsizeiptr size, void* data)
{
	vbo_bind(self);
	glBufferData(self.type, size, data, self.dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}
