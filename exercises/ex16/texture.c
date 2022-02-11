#include "texture.h"

static struct opengl_texture _texture_create_from_pixels(uint8_t* pixels, size_t width, size_t height, size_t channels);
static void _texture_load_pixels(char* path, uint8_t** pixels_out, size_t* width_out, size_t* height_out, size_t* channels_out);

static struct opengl_texture
_texture_create_from_pixels(uint8_t* pixels, size_t width, size_t height, size_t channels)
{
	struct opengl_texture self = {
		.size.x = width,
		.size.y = height
	};

	// Generate and bind the texture.
	glGenTextures(1, &self.handle);
	glBindTexture(GL_TEXTURE_2D, self.handle);

	// Set the texture wrapping/filtering options.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Generate the texture with the image data.
	switch (channels)
	{
		case 3:
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		} break;

		case 4:
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		} break;
	}
	glGenerateMipmap(GL_TEXTURE_2D);

	return(self);
}

static void
_texture_load_pixels(char* path, uint8_t** pixels_out, size_t* width_out, size_t* height_out, size_t* channels_out)
{
	int width, height, channels;

	// OpenGL has texture coordinates with (0, 0) on bottom.
	stbi_set_flip_vertically_on_load(true);

	// Retrieve image data.
	unsigned char* image = stbi_load(path, &width, &height, &channels, 0);
	assert(image != NULL);

	*pixels_out = malloc(width * height * 4);
	memcpy(*pixels_out, image, width * height * 4);
	*width_out = width;
	*height_out = height;
	*channels_out = channels;

	stbi_image_free(image);
}

struct opengl_texture
texture_create(char* path)
{
	uint8_t* pixels;
	size_t width, height, channels;

	_texture_load_pixels(path, &pixels, &width, &height, &channels);
	struct opengl_texture self = _texture_create_from_pixels(pixels, width, height, channels);
	free(pixels);
	return(self);
}

void
texture_destroy(struct opengl_texture self)
{
	glDeleteTextures(1, &self.handle);
}

void
texture_bind(struct opengl_texture self, int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, self.handle);
}
