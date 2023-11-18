#include "engine/vector.h"

/**
 * vector_copy - Copies Vector of SIZE from SRC to DST
 * @dst: Destination
 * @src: Source
 * @size: Size of Vector
 */
void vector_copy(f32 *dst, const f32 *src, const u8 size)
{
	for (u8 i = 0; i < size; i++)
		dst[i] = src[i];
}

/**
 * vector_zero - Sets a Vector of SIZE to zero
 * @v: Vector
 * @size: Size of Vector
 */
void vector_zero(f32 *v, const u8 size)
{
	for (u8 i = 0; i < size; i++)
		v[i] = 0;
}
