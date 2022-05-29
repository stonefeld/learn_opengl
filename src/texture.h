#ifndef TEXTURE_H
#define TEXTURE_H

#include "gfx.h"
#include "types.h"
#include "utils.h"

typedef struct {
	GLuint handle;
	vec2i size;
} Texture;

Texture texture_create(char* path);
void texture_destroy(Texture self);

void texture_bind(Texture self, int unit);

#endif
