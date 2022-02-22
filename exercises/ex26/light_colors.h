#ifndef LIGHT_COLORS_H
#define LIGHT_COLORS_H

#include "gfx.h"

#ifdef LIGHT_DESERT
vec3 background = { 0.75f, 0.52f, 0.3f };

vec3 dirlight_ambient = { 0.3f, 0.24f, 0.14f };
vec3 dirlight_diffuse = { 0.7f, 0.42f, 0.26f };
vec3 dirlight_specular = { 0.5f, 0.5f, 0.5f };

vec3 pointlight_colors[] = {
	{ 1.0f, 0.6f, 0.0f },
	{ 1.0f, 0.0f, 0.0f },
	{ 1.0f, 1.0f, 0.0f },
	{ 0.2f, 0.2f, 1.0f }
};
float pointlight_constant = 1.0f;
float pointlight_linear = 0.09f;
float pointlight_quadratic = 0.032f;

vec3 spotlight_ambient = { 0.0f, 0.0f, 0.0f };
vec3 spotlight_diffuse = { 0.8f, 0.8f, 0.8f };
vec3 spotlight_specular = { 0.8f, 0.8f, 1.0f };
float spotlight_constant = 1.0f;
float spotlight_linear = 0.09f;
float spotlight_quadratic = 0.032f;
float spotlight_cutoff = 12.5f;
float spotlight_cutoff_outer = 13.0f;
#endif

#ifdef LIGHT_FACTORY
vec3 background = { 0.1f, 0.1f, 0.1f };

vec3 dirlight_ambient = { 0.05f, 0.05f, 1.0f };
vec3 dirlight_diffuse = { 0.2f, 0.2f, 0.2f };
vec3 dirlight_specular = { 0.7f, 0.7f, 0.7f };

vec3 pointlight_colors[] = {
	{ 0.2f, 0.2f, 0.6f },
	{ 0.3f, 0.3f, 0.7f },
	{ 0.0f, 0.0f, 0.3f },
	{ 0.4f, 0.4f, 0.4f }
};
float pointlight_constant = 1.0f;
float pointlight_linear = 0.09f;
float pointlight_quadratic = 0.032f;

vec3 spotlight_ambient = { 0.0f, 0.0f, 0.0f };
vec3 spotlight_diffuse = { 1.0f, 1.0f, 1.0f };
vec3 spotlight_specular = { 1.0f, 1.0f, 1.0f };
float spotlight_constant = 1.0f;
float spotlight_linear = 0.009f;
float spotlight_quadratic = 0.0032f;
float spotlight_cutoff = 10.0f;
float spotlight_cutoff_outer = 12.5f;
#endif

#ifdef LIGHT_HORROR
vec3 background = { 0.0f, 0.0f, 0.0f };

vec3 dirlight_ambient = { 0.0f, 0.0f, 0.0f };
vec3 dirlight_diffuse = { 0.05f, 0.05f, 0.05f };
vec3 dirlight_specular = { 0.2f, 0.2f, 0.2f };

vec3 pointlight_colors[] = {
	{ 0.3f, 0.1f, 0.1f },
	{ 0.1f, 0.3f, 0.1f },
	{ 0.1f, 0.1f, 0.3f },
	{ 0.3f, 0.3f, 0.1f }
};
float pointlight_constant = 1.0f;
float pointlight_linear = 0.09f;
float pointlight_quadratic = 0.032f;

vec3 spotlight_ambient = { 0.0f, 0.0f, 0.0f };
vec3 spotlight_diffuse = { 1.0f, 1.0f, 1.0f };
vec3 spotlight_specular = { 1.0f, 1.0f, 1.0f };
float spotlight_constant = 1.0f;
float spotlight_linear = 0.09f;
float spotlight_quadratic = 0.032f;
float spotlight_cutoff = 10.0f;
float spotlight_cutoff_outer = 15.0f;
#endif

#ifdef LIGHT_BIOCHEMICAL_LAB
vec3 background = { 0.9f, 0.9f, 0.9f };

vec3 dirlight_ambient = { 0.5f, 0.5f, 0.5f };
vec3 dirlight_diffuse = { 1.0f, 1.0f, 1.0f };
vec3 dirlight_specular = { 1.0f, 1.0f, 1.0f };

vec3 pointlight_colors[] = {
	{ 0.4f, 0.7f, 0.1f },
	{ 0.4f, 0.7f, 0.1f },
	{ 0.4f, 0.7f, 0.1f },
	{ 0.4f, 0.7f, 0.1f }
};
float pointlight_constant = 1.0f;
float pointlight_linear = 0.07f;
float pointlight_quadratic = 0.017f;

vec3 spotlight_ambient = { 0.0f, 0.0f, 0.0f };
vec3 spotlight_diffuse = { 0.0f, 1.0f, 0.0f };
vec3 spotlight_specular = { 0.0f, 1.0f, 0.0f };
float spotlight_constant = 1.0f;
float spotlight_linear = 0.07f;
float spotlight_quadratic = 0.017f;
float spotlight_cutoff = 7.0f;
float spotlight_cutoff_outer = 10.0f;
#endif

#endif
