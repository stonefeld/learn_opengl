#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

typedef struct
{
	GLFWwindow* handle;
	int width;
	int height;
	char* title;
} opengl_window;

const char* vertex_shader_source = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragment_shader_source = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

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
		 0.5f,  0.5f, 0.0f, // top right
		 0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f  // top left
	};

	// Define the indices to specify the repetition of vertices.
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
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
		printf("Failed to create GLFW window");
		glfwTerminate();
		return(1);
	}
	glfwMakeContextCurrent(window.handle);

	// Initialize GLAD.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD");
		return(1);
	}

	// Specify window Viewport.
	glfwSetFramebufferSizeCallback(window.handle, framebuffer_size_callback);

	// Create a vertex shader.
	unsigned int vertex_shader;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);

	// Attach the shader source code and compile the shader.
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	// Check for errors on shader compilation.
	int success;
	char info_log[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n\t-> %s", info_log);
	}

	// Create a fragment shader.
	unsigned int fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	// Attach the shader source and compile the shader.
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	// Check for errors on shader compilation.
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n\t-> %s", info_log);
	}

	// Create a shader program.
	unsigned int shader_program;
	shader_program = glCreateProgram();

	// Attach the previously compiled shaders to the program and link them.
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	// Check for errors on program linking.
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n\t-> %s", info_log);
	}

	// Delete shader objects, they are no longer needed.
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Create an instance of a vertex buffer object.
	unsigned int vbo;
	glGenBuffers(1, &vbo);

	// Create an instance of a element buffer object.
	unsigned int ebo;
	glGenBuffers(1, &ebo);

	// Create an instance of a vertex array object.
	unsigned int vao;
	glGenVertexArrays(1, &vao);

	// Bind the vao first so everything else gets bound to this vao.
	glBindVertexArray(vao);

	// Bind the vbo and copy the previously defined vertex data into the vbo's memory.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Bind the ebo and copy the previously defined indices into the ebo's memory.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Tell OpenGL how it should interpret the vertex data.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind the vao first and then the vbo.
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

#if 0
	// Enable wireframe mode.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	// Main loop.
	while (!glfwWindowShouldClose(window.handle))
	{
		// Check for input.
		process_input(window.handle);

		// Render some background color.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Activate the shader program, bind the vao and draw the triangle.
		glUseProgram(shader_program);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Check events and swap buffers.
		glfwSwapBuffers(window.handle);
		glfwPollEvents();
	}

	// Terminate the program and close everything properly.
	glfwTerminate();
	return(0);
}
