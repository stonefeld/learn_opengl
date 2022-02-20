#include "shader.h"

static GLuint _shader_compile(const char* path, GLenum type);
static void _shader_log_and_fail(GLint handle, const char* adverb, const char* path,
                                 void (*getlog)(GLuint, GLsizei, GLsizei*, GLchar*),
                                 void (*getiv)(GLuint, GLenum, GLint*));

static GLuint
_shader_compile(const char* path, GLenum type)
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
		_shader_log_and_fail(handle, "compiling", path, glGetShaderInfoLog, glGetShaderiv);
	}

	free(text);
	return(handle);
}

static void
_shader_log_and_fail(GLint handle, const char* adverb, const char* path,
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

struct opengl_shader
shader_create(const char* vertex_path, const char* fragment_path)
{
	struct opengl_shader self;
	self.vertex_handle = _shader_compile(vertex_path, GL_VERTEX_SHADER);
	self.fragment_handle = _shader_compile(fragment_path, GL_FRAGMENT_SHADER);
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
		_shader_log_and_fail(self.handle, "linking", buf, glGetProgramInfoLog, glGetProgramiv);
	}

	return(self);
}

void
shader_destroy(struct opengl_shader self)
{
	glDeleteProgram(self.handle);
	glDeleteShader(self.vertex_handle);
	glDeleteShader(self.fragment_handle);
}

void
shader_bind(struct opengl_shader self)
{
	glUseProgram(self.handle);
}

// ----------------------------------------------------------------------------
void
shader_uniform_int(struct opengl_shader self, const char* name, int v)
{
	glUniform1i(glGetUniformLocation(self.handle, name), v);
}

// ----------------------------------------------------------------------------
void
shader_uniform_float(struct opengl_shader self, const char* name, float v)
{
    glUniform1f(glGetUniformLocation(self.handle, name), v);
}

// ----------------------------------------------------------------------------
void
shader_unifrom_vec2(struct opengl_shader self, const char* name, vec2 v)
{
    glUniform2fv(glGetUniformLocation(self.handle, name), 1, &v[0]);
}
void
shader_unfirom_float2(struct opengl_shader self, const char* name, float x, float y)
{
    glUniform2f(glGetUniformLocation(self.handle, name), x, y);
}

// ----------------------------------------------------------------------------
void
shader_uniform_vec3(struct opengl_shader self, const char* name, vec3 v)
{
    glUniform3fv(glGetUniformLocation(self.handle, name), 1, &v[0]);
}
void
shader_uniform_float3(struct opengl_shader self, const char* name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(self.handle, name), x, y, z);
}

// ----------------------------------------------------------------------------
void
shader_uniform_vec4(struct opengl_shader self, const char* name, vec4 v)
{
    glUniform4fv(glGetUniformLocation(self.handle, name), 1, &v[0]);
}
void
shader_uniform_float4(struct opengl_shader self, const char* name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(self.handle, name), x, y, z, w);
}

// ----------------------------------------------------------------------------
void
shader_uniform_mat2(struct opengl_shader self, const char* name, mat2 v)
{
    glUniformMatrix2fv(glGetUniformLocation(self.handle, name), 1, GL_FALSE, &v[0][0]);
}

// ----------------------------------------------------------------------------
void
shader_uniform_mat3(struct opengl_shader self, const char* name, mat3 v)
{
    glUniformMatrix3fv(glGetUniformLocation(self.handle, name), 1, GL_FALSE, &v[0][0]);
}

// ----------------------------------------------------------------------------
void
shader_uniform_mat4(struct opengl_shader self, const char* name, mat4 v)
{
    glUniformMatrix4fv(glGetUniformLocation(self.handle, name), 1, GL_FALSE, &v[0][0]);
}
