#include "vbo.h"

Vbo
vbo_create(GLint type, bool dynamic)
{
	Vbo self = {
		.type = type,
		.dynamic = dynamic
	};
	glGenBuffers(1, &self.handle);
	return(self);
}

void
vbo_destroy(Vbo self)
{
	glDeleteBuffers(1, &self.handle);
}

void
vbo_bind(Vbo self)
{
	glBindBuffer(self.type, self.handle);
}

void
vbo_unbind(Vbo self)
{
	glBindBuffer(self.type, 0);
}

void
vbo_buffer(Vbo self, GLsizeiptr size, void* data)
{
	vbo_bind(self);
	glBufferData(self.type, size, data, self.dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
}
