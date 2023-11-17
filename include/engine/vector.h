#ifndef _ENGINE_VECTOR_H_
#define _ENGINE_VECTOR_H_

#include "engine/types.h"

/*
 * Base
 */
void vector_copy(f32 *dst, const f32 *src, const u8 size);
void vector_zero(f32 *v, const u8 size);

/*
 * Math
 */
void vector_add(const f32 *a, const f32 *b, f32 *o, const u8 size);
void vector_sub(const f32 *a, const f32 *b, f32 *o, const u8 size);
void vector_scale(f32 *v, const f32 s, const u8 size);
void vector_lerp(const f32 *a, const f32 *b, const f32 t,
		 f32 *o, const u8 size);

/*
 * Trig
 */
void vector3_cross(const f32 *a, const f32 *b, f32 *o);
f32 vector_magnitude_sqr(const f32 *v, const u8 size);
f32 vector_magnitude(const f32 *v, const u8 size);
void vector_normalize(f32 *v, const u8 size);

#endif /* _ENGINE_VECTOR_H_ */
