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
