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

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

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
	struct opengl_vao cube_vao = vao_create();

	// Bind the vao first so everything else gets bound to this vao.
	vao_bind(cube_vao);

	// Bind the cube_vbo and copy the previously defined vertex data into the cube_vbo's memory.
	vbo_bind(cube_vbo);
	vbo_buffer(cube_vbo, sizeof(vertices), vertices);

	// Tell OpenGL how to read the data.
	vao_attr(cube_vao, cube_vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), 0);
	vao_attr(cube_vao, cube_vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), 3 * sizeof(float));
	vao_attr(cube_vao, cube_vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), 6 * sizeof(float));

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

	// Define the point light positions for the scene.
	vec3 pointlight_positions[] = {
		{  0.7f,  0.2f,  2.0f  },
		{  2.3f, -3.3f, -4.0f  },
		{ -4.0f,  2.0f, -12.0f },
		{  0.0f,  0.0f, -3.0f  }
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

		// OTHER OBJECTS
		// Camera position uniform.
		shader_bind(cube_shader);
		shader_uniform_vec3(cube_shader, "u_viewpos", camera.position);

		// Light source lighting uniforms.
		// directional light
		shader_uniform_vec3(cube_shader, "u_dirlight.direction", (vec3){ -0.2f, -1.0f, -0.3f });
		shader_uniform_vec3(cube_shader, "u_dirlight.ambient", (vec3){ 0.05f, 0.05f, 0.05f });
		shader_uniform_vec3(cube_shader, "u_dirlight.diffuse", (vec3){ 0.4f, 0.4f, 0.4f });
		shader_uniform_vec3(cube_shader, "u_dirlight.specular", (vec3){ 0.5f, 0.5f, 0.5f });

		// point light 1
		shader_uniform_vec3(cube_shader, "u_pointlights[0].position", pointlight_positions[0]);
		shader_uniform_vec3(cube_shader, "u_pointlights[0].ambient", (vec3){ 0.05f, 0.05f, 0.05f });
		shader_uniform_vec3(cube_shader, "u_pointlights[0].diffuse", (vec3){ 0.8f, 0.8f, 0.8f });
		shader_uniform_vec3(cube_shader, "u_pointlights[0].specular", (vec3){ 1.0f, 1.0f, 1.0f });
		shader_uniform_float(cube_shader, "u_pointlights[0].constant", 1.0f);
		shader_uniform_float(cube_shader, "u_pointlights[0].linear", 0.09f);
		shader_uniform_float(cube_shader, "u_pointlights[0].quadratic", 0.032f);

		// point light 2
		shader_uniform_vec3(cube_shader, "u_pointlights[1].position", pointlight_positions[1]);
		shader_uniform_vec3(cube_shader, "u_pointlights[1].ambient", (vec3){ 0.05f, 0.05f, 0.05f });
		shader_uniform_vec3(cube_shader, "u_pointlights[1].diffuse", (vec3){ 0.8f, 0.8f, 0.8f });
		shader_uniform_vec3(cube_shader, "u_pointlights[1].specular", (vec3){ 1.0f, 1.0f, 1.0f });
		shader_uniform_float(cube_shader, "u_pointlights[1].constant", 1.0f);
		shader_uniform_float(cube_shader, "u_pointlights[1].linear", 0.09f);
		shader_uniform_float(cube_shader, "u_pointlights[1].quadratic", 0.032f);

		// point light 3
		shader_uniform_vec3(cube_shader, "u_pointlights[2].position", pointlight_positions[2]);
		shader_uniform_vec3(cube_shader, "u_pointlights[2].ambient", (vec3){ 0.05f, 0.05f, 0.05f });
		shader_uniform_vec3(cube_shader, "u_pointlights[2].diffuse", (vec3){ 0.8f, 0.8f, 0.8f });
		shader_uniform_vec3(cube_shader, "u_pointlights[2].specular", (vec3){ 1.0f, 1.0f, 1.0f });
		shader_uniform_float(cube_shader, "u_pointlights[2].constant", 1.0f);
		shader_uniform_float(cube_shader, "u_pointlights[2].linear", 0.09f);
		shader_uniform_float(cube_shader, "u_pointlights[2].quadratic", 0.032f);

		// point light 4
		shader_uniform_vec3(cube_shader, "u_pointlights[3].position", pointlight_positions[3]);
		shader_uniform_vec3(cube_shader, "u_pointlights[3].ambient", (vec3){ 0.05f, 0.05f, 0.05f });
		shader_uniform_vec3(cube_shader, "u_pointlights[3].diffuse", (vec3){ 0.8f, 0.8f, 0.8f });
		shader_uniform_vec3(cube_shader, "u_pointlights[3].specular", (vec3){ 1.0f, 1.0f, 1.0f });
		shader_uniform_float(cube_shader, "u_pointlights[3].constant", 1.0f);
		shader_uniform_float(cube_shader, "u_pointlights[3].linear", 0.09f);
		shader_uniform_float(cube_shader, "u_pointlights[3].quadratic", 0.032f);

		// spot light
		shader_uniform_vec3(cube_shader, "u_spotlight.position", camera.position);
		shader_uniform_vec3(cube_shader, "u_spotlight.direction", camera.front);
		shader_uniform_vec3(cube_shader, "u_spotlight.ambient", (vec3){ 0.05f, 0.05f, 0.05f });
		shader_uniform_vec3(cube_shader, "u_spotlight.diffuse", (vec3){ 0.4f, 0.4f, 0.4f });
		shader_uniform_vec3(cube_shader, "u_spotlight.specular", (vec3){ 0.5f, 0.5f, 0.5f });
		shader_uniform_float(cube_shader, "u_spotlight.constant", 1.0f);
		shader_uniform_float(cube_shader, "u_spotlight.linear", 0.09f);
		shader_uniform_float(cube_shader, "u_spotlight.quadratic", 0.032f);
		shader_uniform_float(cube_shader, "u_spotlight.cutoff", cos(glm_rad(12.5)));
		shader_uniform_float(cube_shader, "u_spotlight.cutoff_outer", cos(glm_rad(17.5)));

		// Bind the cube texture for the diffuse attribute of the material.
		texture_bind(cube_texture, 0);
		texture_bind(cube_specular_map, 1);

		// Material lighting uniforms.
		shader_uniform_int(cube_shader, "u_material.diffuse", 0);
		shader_uniform_int(cube_shader, "u_material.specular", 1);
		shader_uniform_float(cube_shader, "u_material.shininess", 32.0f);

		// Calculate the projection and the view matrices for the light source and all other objects.
		mat4 model;
		mat4 view;
		mat4 projection;
		camera_get_viewmatrix(camera, view);
		glm_mat4_identity(projection);
		glm_perspective(glm_rad(camera.fov), 800.0f / 600.0f, 0.1f, 100.0f, projection);

		// Upload transformation matrices except model matrix.
		shader_uniform_mat4(cube_shader, "u_view", view);
		shader_uniform_mat4(cube_shader, "u_projection", projection);

		// Bind the vao.
		vao_bind(cube_vao);

		// Render the containers.
		for (unsigned int i = 0; i < 10; i++)
		{
			// Calculate the model matrix for each cube.
			glm_mat4_identity(model);
			glm_translate(model, cube_positions[i]);
			float angle = 20.0f * i;
			glm_rotate(model, glm_rad(angle), (vec3){ 1.0f, 0.3f, 0.5f });

			// Upload the model matrix.
			shader_uniform_mat4(cube_shader, "u_model", model);

			// Draw the triangles.
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// LIGHT CUBE
		// Transformation matrices uniforms.
		shader_bind(light_cube_shader);
		shader_uniform_mat4(light_cube_shader, "u_view", view);
		shader_uniform_mat4(light_cube_shader, "u_projection", projection);

		// Bind the light cube vao.
		vao_bind(light_cube_vao);

		// Render the point lights.
		for (unsigned int i = 0; i < 4; i++)
		{
			// Calculate the model matrix for each point light.
			glm_mat4_identity(model);
			glm_translate(model, pointlight_positions[i]);
			glm_scale(model, (vec3){ 0.2f, 0.2f, 0.2f });

			// Upload the model matrix.
			shader_uniform_mat4(light_cube_shader, "u_model", model);

			// Draw the light cube.
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Check events and swap buffers.
		glfwSwapBuffers(window.handle);
		glfwPollEvents();
	}

	// Deallocate buffers and shader programs.
	vao_destroy(cube_vao);
	vbo_destroy(cube_vbo);
	shader_destroy(cube_shader);

	// Terminate the program and close everything properly.
	glfwTerminate();
	return(0);
}
