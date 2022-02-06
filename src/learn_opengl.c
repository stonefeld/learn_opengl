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

vec3 camera_pos = { 0.0f, 0.0f, 3.0f };
vec3 camera_front = { 0.0f, 0.0f, -1.0f };
vec3 camera_target;
vec3 camera_up = { 0.0f, 1.0f, 0.0f };
float camera_fov = 45.0f;

float cursor_pitch = 0.0f;
float cursor_yaw = -90.0f;
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

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	cursor_yaw += xoffset;
	cursor_pitch += yoffset;

	if (cursor_pitch > 89.0f)
		cursor_pitch = 89.0f;
	if (cursor_pitch < -89.0f)
		cursor_pitch = -89.0f;

	vec3 direction = {
		cos(glm_rad(cursor_yaw)) * cos(glm_rad(cursor_pitch)),
		sin(glm_rad(cursor_pitch)),
		sin(glm_rad(cursor_yaw)) * cos(glm_rad(cursor_pitch))
	};

	glm_normalize_to(direction, camera_front);
}

void
process_input(GLFWwindow* handle)
{
	if (glfwGetKey(handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(handle, 1);

	const float camera_speed = 2.5f * delta_time;

	if (glfwGetKey(handle, GLFW_KEY_W) == GLFW_PRESS)
		glm_vec3_muladds(camera_front, camera_speed, camera_pos);

	if (glfwGetKey(handle, GLFW_KEY_S) == GLFW_PRESS)
		glm_vec3_muladds(camera_front, -1.0f * camera_speed, camera_pos);

	if (glfwGetKey(handle, GLFW_KEY_A) == GLFW_PRESS)
	{
		vec3 camera_tmp;
		glm_vec3_cross(camera_front, camera_up, camera_tmp);
		glm_normalize(camera_tmp);
		glm_vec3_muladds(camera_tmp, -1.0f * camera_speed, camera_pos);
	}

	if (glfwGetKey(handle, GLFW_KEY_D) == GLFW_PRESS)
	{
		vec3 camera_tmp;
		glm_vec3_cross(camera_front, camera_up, camera_tmp);
		glm_normalize(camera_tmp);
		glm_vec3_muladds(camera_tmp, camera_speed, camera_pos);
	}
}

void
scroll_callback(GLFWwindow* handle, double xoffset, double yoffset)
{
	camera_fov -= (float)yoffset;
	if (camera_fov < 1.0f)
		camera_fov = 1.0f;
	if (camera_fov > 90.0f)
		camera_fov = 90.0f;
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
	glfwSetCursorPosCallback(window.handle, mouse_callback);
	glfwSetScrollCallback(window.handle, scroll_callback);

	glEnable(GL_DEPTH_TEST); // Enable detph testing.
	glfwSetInputMode(window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Tell OpenGL to hide and capture cursor when focused.

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

	// Create the view matrix and the camera variables.
	mat4 view;

	// Create the projection matrix.
	mat4 projection;

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
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Bind the vao and the textures.
		vao_bind(vao);
		texture_bind(texture1, 0);
		texture_bind(texture2, 1);

		// Calculate the projection.
		glm_mat4_identity(projection);
		glm_perspective(glm_rad(camera_fov), 800.0f / 600.0f, 0.1f, 100.0f, projection);

		// Calculate the camera position.
		glm_vec3_add(camera_pos, camera_front, camera_target);
		glm_lookat(camera_pos, camera_target, camera_up, view);

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
