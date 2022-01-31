#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "utils.h"

typedef struct
{
	GLFWwindow* handle;
	int width;
	int height;
	char* title;
} opengl_window;

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
		// positions         //colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // top
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
	opengl_window window;
	window.width = 800;
	window.height = 600;
	window.title = "Learn OpenGL";

	// Create the window and check if everything is ok.
	window.handle = glfwCreateWindow(window.width, window.height, window.title, NULL, NULL);
	if (window.handle == NULL)
	{
		fprintf(stderr,"Failed to create GLFW window\n");
		glfwTerminate();
		return(1);
	}
	glfwMakeContextCurrent(window.handle);

	// Initialize GLAD.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		fprintf(stderr,"Failed to initialize GLAD\n");
		return(1);
	}

	// Specify window Viewport.
	glfwSetFramebufferSizeCallback(window.handle, framebuffer_size_callback);

	// Retrieve how many vertex attributes are available.
	int nr_attributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nr_attributes);
	printf("Maximum number of vertex attributes supported: %d\n", nr_attributes);

	// Create the shader.
	shader shader_program = shader_create("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");

	// Create an instance of a vertex buffer object.
	unsigned int vbo;
	glGenBuffers(1, &vbo);

	// Create an instance of a vertex array object.
	unsigned int vao;
	glGenVertexArrays(1, &vao);

	// Bind the vao first so everything else gets bound to this vao.
	glBindVertexArray(vao);

	// Bind the vbo and copy the previously defined vertex data into the vbo's memory.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position data.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color data.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind the vao first and then the vbo.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#if 0
	// Enable wireframe mode.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	// Bind the vao.
	glBindVertexArray(vao);

	// Main loop.
	while (!glfwWindowShouldClose(window.handle))
	{
		// Check for input.
		process_input(window.handle);

		// Render some background color.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// First activate the shader program.
		shader_bind(shader_program);
		vec1f offset;
		offset.x = 0.8f;
		shader_uniform_1f(shader_program, "offset", offset);

		// Draw the triangle.
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Check events and swap buffers.
		glfwSwapBuffers(window.handle);
		glfwPollEvents();
	}

	// Deallocate buffers and shader programs.
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	shader_destroy(shader_program);

	// Terminate the program and close everything properly.
	glfwTerminate();
	return(0);
}
