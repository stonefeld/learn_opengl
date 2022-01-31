#ifndef TYPES_H
#define TYPES_H

// Float vectors
typedef struct
{
	float x;
} vec1f;
typedef struct
{
	float x, y;
} vec2f;
typedef struct
{
	float x, y, z;
} vec3f;
typedef struct
{
	float x, y, z, w;
} vec4f;

// Int vectors
typedef struct
{
	int x;
} vec1i;
typedef struct
{
	int x, y;
} vec2i;
typedef struct
{
	int x, y, z;
} vec3i;
typedef struct
{
	int x, y, z, w;
} vec4i;

#endif
