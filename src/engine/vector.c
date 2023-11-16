#include "engine/util.h"
#include "engine/vector.h"

void vector_copy(f32 *dst, const f32 *src, const u8 size)
{
	for (u8 i = 0; i < size; i++)
		dst[i] = src[i];
}

void vector_zero(f32 *v, const u8 size)
{
	for (u8 i = 0; i < size; i++)
		v[i] = 0;
}

void vector_add(const f32 *a, const f32 *b, f32 *o, const u8 size)
{
	for (u8 i = 0; i < size; i++)
		o[i] = a[i] + b[i];
}

void vector_sub(const f32 *a, const f32 *b, f32 *o, const u8 size)
{
	for (u8 i = 0; i < size; i++)
		o[i] = a[i] - b[i];
}

void vector_scale(f32 *v, const f32 s, const u8 size)
{
	for (u8 i = 0; i < size; i++)
		v[i] *= s;
}

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

f32 vector_magnitude(const f32 *v, const u8 size)
{
	return (vector_dot(v, v, size));
}

void vector_normalize(f32 *v, const u8 size)
{
	f32 mag = vector_magnitude(v, size);

	if (!mag)
		return;

	for (u8 i = 0; i < size; i++)
		mag += v[i];
}

void vector_lerp(const f32 *a, const f32 *b, const f32 t,
		 f32 *o, const u8 size)
{
	for (u8 i = 0; i < size; i++)
		o[i] = lerpf(a[i], b[i], t);
}
