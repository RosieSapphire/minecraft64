#include <math.h>
#include <GL/glu.h>

#include "engine/util.h"
#include "engine/vector.h"
#include "engine/camera.h"

/**
 * camera_init - Initializes Camera
 * @c: Camera to Initialize
 */
void camera_init(struct camera *c)
{
	vector_zero(c->eye_last, 3);
	vector_zero(c->eye, 3);
	vector_zero(c->angles_last, 2);
	vector_zero(c->angles, 2);
}

/**
 * camera_get_focus_last - Gets Camera's Focus on Last Frame
 * @c: Camera
 * @out: Focus Last Output
 */
void camera_get_focus_last(const struct camera *c, f32 *out)
{
	const f32 cospl = cosf(c->angles_last[1]);

	vector_copy(out, c->eye_last, 3);
	out[0] += cosf(c->angles_last[0]) * cospl;
	out[1] += sinf(c->angles_last[1]);
	out[2] += sinf(c->angles_last[0]) * cospl;
}

/**
 * camera_get_focus_now - Gets Camera's Focus on Current Frame
 * @c: Camera
 * @out: Focus Now Output
 */
void camera_get_focus_now(const struct camera *c, f32 *out)
{
	const f32 cospl = cosf(c->angles[1]);

	vector_copy(out, c->eye, 3);
	out[0] += cosf(c->angles[0]) * cospl;
	out[1] += sinf(c->angles[1]);
	out[2] += sinf(c->angles[0]) * cospl;
}

/**
 * camera_get_focus_lerp - Gets Camera Value's Focus between Two Frames
 * @eye_lerp: Lerped Eye Position
 * @angles_lerp: Lerped Angles
 * @out: Focus Lerp Output
 */
void camera_get_focus_lerp(f32 *eye_lerp, f32 *angles_lerp, f32 *out)
{
	const f32 cospl = cosf(angles_lerp[1]);

	vector_copy(out, eye_lerp, 3);
	out[0] += cosf(angles_lerp[0]) * cospl;
	out[1] += sinf(angles_lerp[1]);
	out[2] += sinf(angles_lerp[0]) * cospl;
}

/**
 * camera_view_matrix_setup - Sets up OpenGL's View Matrix with Camera
 * @c: Camera
 * @subtick: Delta Between Frames
 */
void camera_view_matrix_setup(const struct camera *c, f32 subtick)
{
	f32 eye_lerp[3];
	f32 angles_lerp[2];
	f32 focus_lerp[3];

	vector_lerp(c->eye_last, c->eye, subtick, eye_lerp, 3);
	vector_lerp(c->angles_last, c->angles, subtick, angles_lerp, 2);
	camera_get_focus_lerp(eye_lerp, angles_lerp, focus_lerp);

	gluLookAt(eye_lerp[0], eye_lerp[1], eye_lerp[2],
		focus_lerp[0], focus_lerp[1], focus_lerp[2],
		0, 1, 0);
}
