#include "camera.h"
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

struct opengl_camera camera;
float cursor_last_x = 400.0f;
float cursor_last_y = 300.0f;
bool cursor_first = true;

vec3 light_pos = { 1.2f, 1.0f, 2.0f };

float delta_time = 0.0f; // Time between current frame and last frame.
float last_frame = 0.0f; // Time of last frame.

void framebuffer_size_callback(GLFWwindow* handle, int widht, int height);
void mouse_callback(GLFWwindow* handle, double xpos, double ypos);
void process_input(GLFWwindow* handle);
void scroll_callback(GLFWwindow* handle, double xoffset, double yoffset);

void
framebuffer_size_callback(GLFWwindow* handle, int width, int height)
{
	glViewport(0, 0, width, height);
}

void
mouse_callback(GLFWwindow* handle, double xpos, double ypos)
{
	if (cursor_first)
	{
		cursor_last_x = xpos;
		cursor_last_y = ypos;
		cursor_first = false;
	}

	float xoffset = xpos - cursor_last_x;
	float yoffset = cursor_last_y - ypos; // Reversed since y-coordinates range from bottom to top.
	cursor_last_x = xpos;
	cursor_last_y = ypos;

	camera_process_mouse(&camera, xoffset, yoffset, true);
}

void
process_input(GLFWwindow* handle)
{
	if (glfwGetKey(handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(handle, 1);

	// Camera movement.
	if (glfwGetKey(handle, GLFW_KEY_W) == GLFW_PRESS)
		camera_process_input(&camera, FORWARD, delta_time);
	if (glfwGetKey(handle, GLFW_KEY_S) == GLFW_PRESS)
		camera_process_input(&camera, BACKWARD, delta_time);
	if (glfwGetKey(handle, GLFW_KEY_A) == GLFW_PRESS)
		camera_process_input(&camera, LEFT, delta_time);
	if (glfwGetKey(handle, GLFW_KEY_D) == GLFW_PRESS)
		camera_process_input(&camera, RIGHT, delta_time);
}

void
scroll_callback(GLFWwindow* handle, double xoffset, double yoffset)
{
	camera_process_scroll(&camera, yoffset);
}

int
main(int argc, char* argv[])
{
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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

	// Set OpenGL callbacks.
	glfwSetFramebufferSizeCallback(window.handle, framebuffer_size_callback);
	glfwSetCursorPosCallback(window.handle, mouse_callback);
	glfwSetScrollCallback(window.handle, scroll_callback);

	glEnable(GL_DEPTH_TEST); // Enable detph testing.
	glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Tell OpenGL to hide and capture cursor when focused.

	// Create the shaders.
	struct opengl_shader cube_shader = shader_create("assets/shaders/lighting_vert.glsl", "assets/shaders/lighting_frag.glsl");
	struct opengl_shader light_cube_shader = shader_create("assets/shaders/light_cube_vert.glsl", "assets/shaders/light_cube_frag.glsl");

	// Create an instance of a vertex buffer object.
	struct opengl_vbo cube_vbo = vbo_create(GL_ARRAY_BUFFER, false);

	// Create an instance of a vertex array object.
	struct opengl_vao vao = vao_create();

	// Bind the vao first so everything else gets bound to this vao.
	vao_bind(vao);

	// Bind the cube_vbo and copy the previously defined vertex data into the cube_vbo's memory.
	vbo_bind(cube_vbo);
	vbo_buffer(cube_vbo, sizeof(vertices), vertices);

	// Tell OpenGL how to read the data.
	vao_attr(vao, cube_vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), 0);
	vao_attr(vao, cube_vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), 3 * sizeof(float));
	vao_attr(vao, cube_vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), 6 * sizeof(float));

#if 0
	// Enable wireframe mode.
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	// Create the light source vao.
	struct opengl_vao light_cube_vao = vao_create();

	// Bind the previous cube_vbo and the new vao.
	vao_bind(light_cube_vao);
	vbo_bind(cube_vbo);

	// Specify the light cube's vertex positions.
	vao_attr(light_cube_vao, cube_vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), 0);

	// Unbind vao first then cube_vbo and ebo.
	vao_unbind();
	vbo_unbind(cube_vbo);

	// Create the new cube texture.
	struct opengl_texture cube_texture = texture_create("assets/images/container2.png");
	struct opengl_texture cube_specular_map = texture_create("assets/images/container2_specular.png");

	// Create the camera with default values.
	camera = camera_create_default();

	// Define the cube positions for the scene.
	vec3 cube_positions[] = {
		{  0.0f,  0.0f,  0.0f  },
		{  2.0f,  5.0f, -15.0f },
		{ -1.5f, -2.2f, -2.5f  },
		{ -3.8f, -2.0f, -12.3f },
		{  2.4f, -0.4f, -3.5f  },
		{ -1.7f,  3.0f, -7.5f  },
		{  1.3f, -2.0f, -2.5f  },
		{  1.5f,  2.0f, -2.5f  },
		{  1.5f,  0.2f, -1.5f  },
		{ -1.3f,  1.0f, -1.5f  }
	};

	// Main loop.
	while (!glfwWindowShouldClose(window.handle))
	{
		// Calculate delta_time.
		float current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		// Check for input.
		process_input(window.handle);

		// Render some background color.
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate the projection and the view matrices for the light source and all other objects.
		mat4 model;
		mat4 view;
		mat4 projection;
		camera_get_viewmatrix(camera, view);
		glm_mat4_identity(projection);
		glm_perspective(glm_rad(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f, projection);

		// OTHER OBJECTS
		// Bind the vao.
		vao_bind(vao);

		// Camera position uniform.
		shader_bind(cube_shader);
		shader_uniform_vec3(cube_shader, "viewPos", camera.position);

		// Light source lighting uniforms.
		// shader_uniform_vec3(cube_shader, "light.position", light_pos);
		// shader_uniform_vec3(cube_shader, "light.direction", (vec3){ -0.2f, -1.0f, -0.3f });
		shader_uniform_vec3(cube_shader, "light.position", camera.position);
		shader_uniform_vec3(cube_shader, "light.direction", camera.front);
		shader_uniform_float(cube_shader, "light.cutOff", cos(glm_rad(12.5f)));
		shader_uniform_float(cube_shader, "light.cutOffOuter", cos(glm_rad(17.5f)));
		shader_uniform_vec3(cube_shader, "light.ambient", (vec3){ 0.1f, 0.1f, 0.1f });
		shader_uniform_vec3(cube_shader, "light.diffuse", (vec3){ 0.8f, 0.8f, 0.8f });
		shader_uniform_vec3(cube_shader, "light.specular", (vec3){ 1.0f, 1.0f, 1.0f });
		shader_uniform_float(cube_shader, "light.constant", 1.0f);
		shader_uniform_float(cube_shader, "light.linear", 0.09f);
		shader_uniform_float(cube_shader, "light.quadratic", 0.032f);

		// Bind the cube texture for the diffuse attribute of the material.
		texture_bind(cube_texture, 0);
		texture_bind(cube_specular_map, 1);

		// Material lighting uniforms.
		shader_uniform_int(cube_shader, "material.diffuse", 0);
		shader_uniform_int(cube_shader, "material.specular", 1);
		shader_uniform_float(cube_shader, "material.shininess", 32.0f);

		// Upload transformation matrices except model matrix.
		shader_uniform_mat4(cube_shader, "view", view);
		shader_uniform_mat4(cube_shader, "projection", projection);

		for (unsigned int i = 0; i < 10; i++)
		{
			// Calculate the model matrix for each cube.
			glm_mat4_identity(model);
			glm_translate(model, cube_positions[i]);
			float angle = 20.0f * i;
			glm_rotate(model, glm_rad(angle), (vec3){ 1.0f, 0.3f, 0.5f });

			// Upload the model matix.
			shader_uniform_mat4(cube_shader, "model", model);

			// Draw the triangles.
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

#if 0
		// LIGHT CUBE
		// Bind the light cube vao.
		vao_bind(light_cube_vao);

		// Calculate the model matrix for the light source.
		glm_mat4_identity(model);
		glm_translate(model, light_pos);
		glm_scale(model, (vec3){ 0.2f, 0.2f, 0.2f });

		shader_bind(light_cube_shader);
		shader_uniform_mat4(light_cube_shader, "model", model);
		shader_uniform_mat4(light_cube_shader, "view", view);
		shader_uniform_mat4(light_cube_shader, "projection", projection);

		// Draw the light cube.
		glDrawArrays(GL_TRIANGLES, 0, 36);
#endif

		// Check events and swap buffers.
		glfwSwapBuffers(window.handle);
		glfwPollEvents();
	}

	// Deallocate buffers and shader programs.
	vao_destroy(vao);
	vbo_destroy(cube_vbo);
	shader_destroy(cube_shader);

	// Terminate the program and close everything properly.
	glfwTerminate();
	return(0);
}
