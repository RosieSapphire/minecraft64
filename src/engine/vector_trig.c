#include <math.h>

#include "engine/vector.h"

/**
 * vector3_cross - Vector 3 Cross Product
 * @a: Vector A
 * @b: Vector B
 * @o: Vector Output
 */
void vector3_cross(const f32 *a, const f32 *b, f32 *o)
{
	o[0] = a[1] * b[2] - a[2] * b[1];
	o[1] = a[2] * b[0] - a[0] * b[2];
	o[2] = a[0] * b[1] - a[1] * b[0];
}

/**
 * vector_dot - Vector Dot Product
 * @a: Vector A
 * @b: Vector B
 * @size: Size of Vectors
 *
 * Return: Vector A and B's Dot Product
 */
f32 vector_dot(const f32 *a, const f32 *b, const u8 size)
{
	f32 dot = 0.0f;

	for (u8 i = 0; i < size; i++)
		dot += a[i] * b[i];

	return (dot);
}

/**
 * vector_magnitude_sqr - Vector Magnitude Squared
 * @v: Vector
 * @size: Size of Vector
 *
 * Return: Vector's Length Squared
 */
f32 vector_magnitude_sqr(const f32 *v, const u8 size)
{
	return (vector_dot(v, v, size));
}

/**
 * vector_magnitude - Vector Magnitude
 * @v: Vector
 * @size: Size of Vector
 *
 * Return: Vector's Length
 */
f32 vector_magnitude(const f32 *v, const u8 size)
{
	return (sqrtf(vector_magnitude_sqr(v, size)));
}

/**
 * vector_normalize - Vector Normalize Function
 * @v: Vector
 * @size: Size of Vector
 */
void vector_normalize(f32 *v, const u8 size)
{
	f32 mag = vector_magnitude(v, size);

	if (!mag)
		return;

	for (u8 i = 0; i < size; i++)
		mag += v[i];
}
