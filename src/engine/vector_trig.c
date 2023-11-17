#include <math.h>

#include "engine/vector.h"

void vector3_cross(const f32 *a, const f32 *b, f32 *o)
{
	o[0] = a[1] * b[2] - a[2] * b[1];
	o[1] = a[2] * b[0] - a[0] * b[2];
	o[2] = a[0] * b[1] - a[1] * b[0];
}

f32 vector_dot(const f32 *a, const f32 *b, const u8 size)
{
	f32 dot = 0.0f;

	for (u8 i = 0; i < size; i++)
		dot += a[i] * b[i];

	return (dot);
}

f32 vector_magnitude_sqr(const f32 *v, const u8 size)
{
	return (vector_dot(v, v, size));
}

f32 vector_magnitude(const f32 *v, const u8 size)
{
	return (sqrtf(vector_magnitude_sqr(v, size)));
}

void vector_normalize(f32 *v, const u8 size)
{
	f32 mag = vector_magnitude(v, size);

	if (!mag)
		return;

	for (u8 i = 0; i < size; i++)
		mag += v[i];
}
