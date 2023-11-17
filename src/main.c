#include <GL/gl.h>
#include <GL/gl_integration.h>
#include <libdragon.h>

#define TICKRATE 24
#define DELTATIME (1.0f / (f32)TICKRATE)
#define DELTATICKS (DELTATIME * TICKS_PER_SECOND)

#include "engine/profiler.h"
#include "engine/block.h"
#include "engine/config.h"
#include "engine/camera.h"
#include "engine/iparms.h"

static surface_t *color_buffer;
static surface_t depth_buffer;
static struct camera cam;
static u32 ticks_last, ticks_now, ticks_delta, ticks_accum;

static void _init(void)
{
	camera_init(&cam);

	/*
	 * debug_init_isviewer();
	 * debug_init_usblog();
	 */
	display_init(CONF_RESOLUTION, CONF_DEPTH,
			CONF_NUM_BUFFERS, CONF_GAMMA, CONF_FILTER);
	rdpq_init();
	joypad_init();
	gl_init();
	dfs_init(DFS_DEFAULT_LOCATION);
	blocks_init();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(
		-CONF_NEAR * CONF_ASPECT, CONF_NEAR * CONF_ASPECT,
		-CONF_NEAR, CONF_NEAR, CONF_NEAR, CONF_FAR);

	depth_buffer = surface_alloc(FMT_RGBA16, CONF_WIDTH, CONF_HEIGHT);

}

static void _update(void)
{
	ticks_now = get_ticks();
	ticks_delta = TICKS_DISTANCE(ticks_last, ticks_now);
	ticks_last = ticks_now;
	ticks_accum += ticks_delta;

	while (ticks_accum >= DELTATICKS)
	{
		joypad_poll();

		const struct input_parms iparms = {
			joypad_get_buttons_pressed(JOYPAD_PORT_1),
			joypad_get_buttons_held(JOYPAD_PORT_1),
			joypad_get_inputs(JOYPAD_PORT_1),
		};

		blocks_update(&cam, iparms);
		ticks_accum -= DELTATICKS;
	}
}

/**
 * main - Main Function
 *
 * Return: 0 if Successful
 */
int main(void)
{
	_init();

	ticks_last = get_ticks();
	ticks_accum = 0;
	while (1)
	{
		profiler_begin("_update()");
		_update();
		profiler_end_print();

		color_buffer = display_get();
		rdpq_attach(color_buffer, &depth_buffer);
		gl_context_begin();

		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		f32 subtick = (f32)ticks_accum / (f32)DELTATICKS;

		profiler_begin("blocks_draw()");
		blocks_draw(subtick, &cam);
		profiler_end_print();

		gl_context_end();
		rdpq_detach_show();
	}

	return (0);
}
