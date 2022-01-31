#ifndef TEXTURE_H
#define TEXTURE_H

#include <gfx.h>
#include <types.h>
#include <utils.h>

struct opengl_texture
{
	GLuint handle;
	vec2i size;
};

struct opengl_texture texture_create(char* path);
void texture_destroy(struct opengl_texture self);

void texture_bind(struct opengl_texture self, int unit);

#endif
