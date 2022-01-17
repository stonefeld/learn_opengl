#include "shader.h"
#include <stdlib.h>

static int _compile(const char* path, GLenum type);
static void _log_and_fail(GLint handle, const char* adverb, const char* path,
                          void (*getlog)(GLuint, GLsizei, GLsizei*, GLchar*),
                          void (*getiv)(GLuint, GLenum, GLint*));

static void
_log_and_fail(GLint handle, const char* adverb, const char* path,
              void (*getlog)(GLuint, GLsizei, GLsizei*, GLchar*),
              void (*getiv)(GLuint, GLenum, GLint*))
{
	GLint loglen;
	getiv(handle, GL_INFO_LOG_LENGTH, &loglen);

	char* logtext = calloc(1, loglen);
	getlog(handle, loglen, NULL, logtext);
	fprintf(stderr, "Error %s shader at %s:\n\t-> %s", adverb, path, logtext);

	free(logtext);
	exit(1);
}

static int
_compile(const char* path, GLenum type)
{
	FILE* f;
	char* text;
	long len;

	// Open the file.
	f = fopen(path, "rb");
	if (f == NULL)
	{
		fprintf(stderr, "Error loading shader at %s\n", path);
		exit(1);
	}

	// Move file pointer to end of the file.
	fseek(f, 0, SEEK_END);
	len = ftell(f);
	assert(len > 0);

	// Move file pointer to the begginning of the file.
	fseek(f, 0, SEEK_SET);
	text = calloc(1, len);
	assert(text != NULL);

	// Read the contents of the file.
	fread(text, 1, len, f);
	assert(strlen(text) > 0);

	// Close the file.
	fclose(f);

	// Create and compile the shader.
	GLuint handle = glCreateShader(type);
	glShaderSource(handle, 1, (const GLchar* const*)&text, (const GLint*)&len);
	glCompileShader(handle);

	// Check for errors.
	GLint compiled;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		_log_and_fail(handle, "compiling", path, glGetShaderInfoLog, glGetShaderiv);
	}

	free(text);
	return(handle);
}

shader
shader_create(const char* vertex_path, const char* fragment_path)
{
	shader self;
	self.vertex_handle = _compile(vertex_path, GL_VERTEX_SHADER);
	self.fragment_handle = _compile(fragment_path, GL_FRAGMENT_SHADER);
	self.handle = glCreateProgram();

	// Link shader program.
	glAttachShader(self.handle, self.vertex_handle);
	glAttachShader(self.handle, self.fragment_handle);
	glLinkProgram(self.handle);

	// Check for errors.
	GLint linked;
	glGetProgramiv(self.handle, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		char buf[512];
		snprintf(buf, 512, "[%s, %s]", vertex_path, fragment_path);
		_log_and_fail(self.handle, "linking", buf, glGetProgramInfoLog, glGetProgramiv);
	}

	return(self);
}

void
shader_destroy(shader self)
{
	glDeleteProgram(self.handle);
	glDeleteShader(self.vertex_handle);
	glDeleteShader(self.fragment_handle);
}

void
shader_bind(shader self)
{
	glUseProgram(self.handle);
}

void
shader_uniform_1i(shader self, const char* name, vec1i v)
{
	glUniform1i(glGetUniformLocation(self.handle, name), v.x);
}

void
shader_uniform_2i(shader self, const char* name, vec2i v)
{
	glUniform2i(glGetUniformLocation(self.handle, name), v.x, v.y);
}

void
shader_uniform_3i(shader self, const char* name, vec3i v)
{
	glUniform3i(glGetUniformLocation(self.handle, name), v.x, v.y, v.z);
}

void
shader_uniform_4i(shader self, const char* name, vec4i v)
{

	glUniform4i(glGetUniformLocation(self.handle, name), v.x, v.y, v.z, v.w);
}

void
shader_uniform_1f(shader self, const char* name, vec1f v)
{
	glUniform1f(glGetUniformLocation(self.handle, name), v.x);
}

void
shader_uniform_2f(shader self, const char* name, vec2f v)
{
	glUniform2f(glGetUniformLocation(self.handle, name), v.x, v.y);
}

void
shader_uniform_3f(shader self, const char* name, vec3f v)
{
	glUniform3f(glGetUniformLocation(self.handle, name), v.x, v.y, v.z);
}

void
shader_uniform_4f(shader self, const char* name, vec4f v)
{
	glUniform4f(glGetUniformLocation(self.handle, name), v.x, v.y, v.z, v.w);
}
