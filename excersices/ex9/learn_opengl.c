#include "gfx.h"
#include "shader.h"
#include "utils.h"
#include "vao.h"
#include "vbo.h"

struct opengl_window
{
	GLFWwindow* handle;
	int width;
	int height;
	char* title;
};

void framebuffer_size_callback(GLFWwindow* handle, int widht, int height);
void process_input(GLFWwindow* handle);

void
framebuffer_size_callback(GLFWwindow* handle, int width, int height)
{
	glViewport(0, 0, width, height);
}

void
process_input(GLFWwindow* handle)
{
	if (glfwGetKey(handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(handle, 1);
}

int
main(int argc, char* argv[])
{
	// Define vertices for the triangles to draw a rectangle.
	float vertices[] = {
		// positions         // colors          // texture
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f  // top left
	};

	// Define indices for the element buffer of each triangle.
	unsigned int indices[] = {
		0, 1, 3, // top triangle
		1, 2, 3  // bottom triangle
	};

	// Initialize GLFW.
	glfwInit();

	// Specify GL version and profile.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Enable floating window and disable resizing.
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

	// Create an opnegl_window instance and specify its attributes.
	struct opengl_window window;
	window.width = 800;
	window.height = 600;
	window.title = "Learn OpenGL";

	// Create the window and check if everything is ok.
	window.handle = glfwCreateWindow(window.width, window.height, window.title, NULL, NULL);
	if (window.handle == NULL)
	{
		fprintf(stderr, "%s", "Failed to create GLFW window\n");
		glfwTerminate();
		return(1);
	}
	glfwMakeContextCurrent(window.handle);

	// Initialize GLAD.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr, "%s", "Failed to initialize GLAD\n");
		return(1);
	}

	// Specify window viewport.
	glfwSetFramebufferSizeCallback(window.handle, framebuffer_size_callback);

	// Generate and bind the texture1.
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Set the texture wrapping/filtering options.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Flip image vertically.
	stbi_set_flip_vertically_on_load(true);

	// Retrieve image data.
	int width, height, nr_channels;
	unsigned char* data = stbi_load("assets/images/container.jpg", &width, &height, &nr_channels, 0);

	// Generate the texture with the image data.
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		fprintf(stderr, "%s", "Failed to load texture\n");
	}

	// Free image memory.
	stbi_image_free(data);

	// Generate and bind the texture2.
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// Set the texture wrapping/filtering options.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Retrieve image data.
	data = stbi_load("assets/images/awesomeface.png", &width, &height, &nr_channels, 0);

	// Generate the texture with the image data.
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		fprintf(stderr, "%s", "Failed to load texture\n");
	}

	// Free image memory.
	stbi_image_free(data);

	// Create the shader.
	struct opengl_shader shader_program = shader_create("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");

	// Create an instance of a vertex buffer object.
	struct opengl_vbo vbo = vbo_create(GL_ARRAY_BUFFER, false);

	// Create an instance of a element buffer object.
	struct opengl_vbo ebo = vbo_create(GL_ELEMENT_ARRAY_BUFFER, false);

	// Create an instance of a vertex array object.
	struct opengl_vao vao = vao_create();

	// Bind the vao first so everything else gets bound to this vao.
	vao_bind(vao);

	// Bind the vbo and copy the previously defined vertex data into the vbo's memory.
	vbo_bind(vbo);
	vbo_buffer(vbo, sizeof(vertices), vertices);

	// Bind the ebo and copy the previously defined indices data into the ebo's memory.
	vbo_bind(ebo);
	vbo_buffer(ebo, sizeof(indices), indices);

	// Position data.
	vao_attr(vao, vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), 0);

	// Color data.
	vao_attr(vao, vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), 3 * sizeof(float));

	// Texture data.
	vao_attr(vao, vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), 6 * sizeof(float));

#if 0
	// Enable wireframe mode.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	// Unbind vao first then vbo and ebo.
	vao_unbind();
	vbo_unbind(vbo);
	vbo_unbind(ebo);

	// Tell OpenGL which location each texture corresponds to.
	shader_bind(shader_program);
	shader_uniform_1i(shader_program, "texture1", 0);
	shader_uniform_1i(shader_program, "texture2", 1);

	// Main loop.
	while (!glfwWindowShouldClose(window.handle))
	{
		// Check for input.
		process_input(window.handle);

		// Render some background color.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Bind the vao and the textures.
		vao_bind(vao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// First activate the shader program.
		shader_bind(shader_program);

		// Draw the triangles.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Check events and swap buffers.
		glfwSwapBuffers(window.handle);
		glfwPollEvents();
	}

	// Deallocate buffers and shader programs.
	vao_destroy(vao);
	vbo_destroy(vbo);
	shader_destroy(shader_program);

	// Terminate the program and close everything properly.
	glfwTerminate();
	return(0);
}
