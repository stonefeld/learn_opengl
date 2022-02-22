#ifndef CAMERA_H
#define CAMERA_H

#include "gfx.h"
#include "utils.h"

enum camera_movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

struct opengl_camera
{
	mat4 viewmatrix;

	vec3 position;
	vec3 front;
	vec3 up;
	vec3 right;
	vec3 world_up;

	float yaw;
	float pitch;
	float speed;
	float sensitivity;
	float fov;
};

struct opengl_camera camera_create(vec3 position, vec3 up, float yaw, float pitch, float speed, float sensitivity, float fov);
struct opengl_camera camera_create_default(void);

void camera_get_viewmatrix(struct opengl_camera self, mat4 view);
void camera_process_input(struct opengl_camera* self, enum camera_movement direction, float delta_time);
void camera_process_mouse(struct opengl_camera* self, float xoffset, float yoffset, GLboolean constrain_pitch);
void camera_process_scroll(struct opengl_camera* self, double yoffset);

#endif
