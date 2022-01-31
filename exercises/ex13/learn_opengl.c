#include "gfx.h"
#include "shader.h"
#include "texture.h"
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
		// coords             // texture
		// first face
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		// second face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		// third face
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		// fourth face
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		// fifth face
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		// sixth face
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// Initialize GLFW.
	if (!glfwInit())
    {
        fprintf(stderr, "%s", "Failed to initialize GLFW\n");
        return(1);
    }

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

	// Enable detph testing.
	glEnable(GL_DEPTH_TEST);

	// Create the shader.
	struct opengl_shader shader_program = shader_create("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");

	// Create an instance of a vertex buffer object.
	struct opengl_vbo vbo = vbo_create(GL_ARRAY_BUFFER, false);

	// Create an instance of a vertex array object.
	struct opengl_vao vao = vao_create();

	// Bind the vao first so everything else gets bound to this vao.
	vao_bind(vao);

	// Bind the vbo and copy the previously defined vertex data into the vbo's memory.
	vbo_bind(vbo);
	vbo_buffer(vbo, sizeof(vertices), vertices);

	// Position data.
	vao_attr(vao, vbo, 0, 3, GL_FLOAT, 5 * sizeof(float), 0);

	// Texture data.
	vao_attr(vao, vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), 3 * sizeof(float));

#if 0
	// Enable wireframe mode.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	// Unbind vao first then vbo and ebo.
	vao_unbind();
	vbo_unbind(vbo);

	// Instantiate the textures.
	struct opengl_texture texture1 = texture_create("assets/images/container.jpg");
	struct opengl_texture texture2 = texture_create("assets/images/awesomeface.png");

	// Tell OpenGL which location each texture corresponds to.
	shader_bind(shader_program);
	shader_uniform_int(shader_program, "texture1", 0);
	shader_uniform_int(shader_program, "texture2", 1);

	// Create the array of cube positions.
	vec3 cube_positions[] = {
		{ 0.0f, 0.0f, 0.0f },
		{ 2.0f, 5.0f, -15.0f },
		{ -1.5f, -2.2f, -2.5f },
		{ -3.8f, -2.0f, -12.3f },
		{ 2.4f, -0.4f, -3.5f },
		{ -1.7f, 3.0f, -7.5f },
		{ 1.3f, -2.0f, -2.5f },
		{ 1.5f, 2.0f, -2.5f },
		{ 1.5f, 0.2f, -1.5f },
		{ -1.3f, 1.0f, -1.5f }
	};

	// Create the model matrix.
	mat4 model;

	// Create the view matrix.
	mat4 view;
	glm_mat4_identity(view);
	glm_translate(view, (vec3){ 0.0f, 0.0f, -3.0f });

	// Create the projection matrix.
	mat4 projection;
	glm_mat4_identity(projection);
	glm_perspective(glm_rad(90.0f), 600.0f / 200.0f, 0.1f, 100.0f, projection);

	// Main loop.
	while (!glfwWindowShouldClose(window.handle))
	{
		// Check for input.
		process_input(window.handle);

		// Render some background color.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind the vao and the textures.
		vao_bind(vao);
		texture_bind(texture1, 0);
		texture_bind(texture2, 1);

		// Pass the transformation matrices to the shader.
		shader_bind(shader_program);
		shader_uniform_mat4(shader_program, "view", view);
		shader_uniform_mat4(shader_program, "projection", projection);

		for (unsigned int i = 0; i < 10; i++)
		{
			float angle = 20.0f * i;

			glm_mat4_identity(model);
			glm_translate(model, cube_positions[i]);
			glm_rotate(model, glm_rad(angle), (vec3){ 1.0f, 0.3f, 0.5f });
			shader_uniform_mat4(shader_program, "model", model);

			// Draw the triangles.
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

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
