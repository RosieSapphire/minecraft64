#include "engine/util.h"
#include "engine/vector.h"

/**
 * vector_add - Vector Add Function
 * @a: Vector A
 * @b: Vector B
 * @o: Vector Output
 * @size: Size of Vector
 */
void vector_add(const f32 *a, const f32 *b, f32 *o, const u8 size)
{
	for (u8 i = 0; i < size; i++)
		o[i] = a[i] + b[i];
}

/**
 * vector_sub - Vector Subtract Function
 * @a: Vector A
 * @b: Vector B
 * @o: Vector Output
 * @size: Size of Vector
 */
void vector_sub(const f32 *a, const f32 *b, f32 *o, const u8 size)
{
	for (u8 i = 0; i < size; i++)
		o[i] = a[i] - b[i];
}

/**
 * vector_scale - Vector Scaling Function
 * @v: Vector
 * @s: Scalar
 * @size: Size of Vector
 */
void vector_scale(f32 *v, const f32 s, const u8 size)
{
	for (u8 i = 0; i < size; i++)
		v[i] *= s;
}

/**
 * vector_lerp - Vector Lerping Function
 * @a: Vector A
 * @b: Vector B
 * @t: Interpolation
 * @o: Output Vector
 * @size: Size of Vector
 */
void vector_lerp(const f32 *a, const f32 *b, const f32 t,
		 f32 *o, const u8 size)
{
	for (u8 i = 0; i < size; i++)
		o[i] = lerpf(a[i], b[i], t);
}
