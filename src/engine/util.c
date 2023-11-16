#include "engine/util.h"

f32 clampf(f32 x, f32 min, f32 max)
{
	if (x < min)
		return (min);

	if (x > max)
		return (max);

	return (x);
}

f32 lerpf(f32 a, f32 b, f32 t)
{
	return (a + (b - a) * t);
}
