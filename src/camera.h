#ifndef CAMERA_H
#define CAMERA_H

#include "gfx.h"
#include "utils.h"

typedef enum {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
} Movement;

typedef struct {
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
} Camera;

Camera camera_create(vec3 position, vec3 up, float yaw, float pitch, float speed, float sensitivity, float fov);
Camera camera_create_default(void);

void camera_get_viewmatrix(Camera self, mat4 view);
void camera_process_input(Camera *self, Movement direction, float delta_time);
void camera_process_mouse(Camera *self, float xoffset, float yoffset, GLboolean constrain_pitch);
void camera_process_scroll(Camera *self, double yoffset);

#endif
