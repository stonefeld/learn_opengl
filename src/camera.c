#include "camera.h"

static void _camera_update_vectors(Camera *self);

static void
_camera_update_vectors(Camera *self)
{
	vec3 direction = {
		cos(glm_rad(self->yaw)) * cos(glm_rad(self->pitch)),
		sin(glm_rad(self->pitch)),
		sin(glm_rad(self->yaw)) * cos(glm_rad(self->pitch))
	};

	glm_normalize_to(direction, self->front);

	glm_vec3_cross(self->front, self->world_up, self->right);
	glm_normalize(self->right);

	glm_vec3_cross(self->right, self->front, self->up);
	glm_normalize(self->up);
}

Camera
camera_create(vec3 position, vec3 up, float yaw, float pitch, float speed, float sensitivity, float fov)
{
	Camera self = {
		.yaw = yaw,
		.pitch = pitch,
		.speed = speed,
		.sensitivity = sensitivity,
		.fov = fov
	};
	strncpy((char*)self.position, (char*)position, 3);
	strncpy((char*)self.world_up, (char*)up, 3);
	_camera_update_vectors(&self);
	return(self);
}

Camera
camera_create_default()
{
	Camera self = {
		.position = { 0.0f, 0.0f, 3.0f },
		.world_up = { 0.0f, 1.0f, 0.0f },
		.yaw = -90.0f,
		.pitch = 0.0f,
		.speed = 2.5f,
		.sensitivity = 0.1f,
		.fov = 45.0f,
	};
	_camera_update_vectors(&self);
	return(self);
}

void
camera_get_viewmatrix(Camera self, mat4 view)
{
	vec3 target;

	glm_vec3_add(self.position, self.front, target);
	glm_lookat(self.position, target, self.world_up, view);
}

void
camera_process_input(Camera *self, Movement direction, float delta_time)
{
	float velocity = self->speed * delta_time;

	switch (direction) {
		case FORWARD: {
			glm_vec3_muladds(self->front, velocity, self->position);
		} break;

		case BACKWARD: {
			glm_vec3_muladds(self->front, -1.0f * velocity, self->position);
		} break;

		case LEFT: {
			glm_vec3_muladds(self->right, -1.0f * velocity, self->position);
		} break;

		case RIGHT: {
			glm_vec3_muladds(self->right, velocity, self->position);
		} break;
	}
}

void
camera_process_mouse(Camera *self, float xoffset, float yoffset, GLboolean constrain_pitch)
{
	xoffset *= self->sensitivity;
	yoffset *= self->sensitivity;

	self->yaw += xoffset;
	self->pitch += yoffset;

	if (constrain_pitch) {
		if (self->pitch > 89.0f)
			self->pitch = 89.0f;
		else if (self->pitch < -89.0f)
			self->pitch = -89.0f;
	}

	_camera_update_vectors(self);
}

void
camera_process_scroll(Camera *self, double yoffset)
{
	self->fov -= (float)yoffset;

	if (self->fov < 1.0f)
		self->fov = 1.0f;
	else if (self->fov > 90.0f)
		self->fov = 90.0f;
}
