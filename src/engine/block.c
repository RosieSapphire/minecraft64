#include <GL/gl.h>
#include <GL/glu.h>

#include "engine/profiler.h"
#include "engine/util.h"
#include "engine/vertex.h"
#include "engine/block.h"
#include "engine/texture.h"
#include "engine/vector.h"

struct block blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];

/**
 * blocks_init - Initializes Blocks
 */
void blocks_init(void)
{
	for (int z = 0; z < CHUNK_Z; z++)
	{
		for (int y = 0; y < CHUNK_Y; y++)
		{
			for (int x = 0; x < CHUNK_X; x++)
			{
				blocks[x][y][z].pos[0] = x;
				blocks[x][y][z].pos[1] = y;
				blocks[x][y][z].pos[2] = z;
				blocks[x][y][z].flags = BLOCK_IS_ACTIVE;
			}
		}
	}

	block_dl_textures_load();
	blocks_dl_build();
}

/**
 * blocks_update - Update Blocks
 * @c: Camera
 * @iparms: Input Parameteres
 */
void blocks_update(struct camera *c, const struct input_parms iparms)
{
	/*
	 * angle changing
	 */
	vector_copy(c->angles_last, c->angles, 2);
	c->angles[0] += (f32)iparms.stick.stick_x * 0.001f;
	c->angles[1] -= (f32)iparms.stick.stick_y * 0.001f;

	/*
	 * position changing
	 */
	f32 focus[3], forw[3] = {0}, side[3] = {0}, move[3] = {0};
	const f32 move_forw =
		(f32)(iparms.held.c_up - iparms.held.c_down) * 0.1f;
	const f32 move_side =
		(f32)(iparms.held.c_right - iparms.held.c_left) * 0.1f;
	const u8 go_fast = iparms.held.z + 1;

	camera_get_focus_now(c, focus);
	vector_sub(focus, c->eye, forw, 3);
	vector3_cross(forw, (f32[3]) {0, 1, 0}, side);
	vector_scale(forw, move_forw * go_fast, 3);
	vector_scale(side, move_side * go_fast, 3);

	vector_copy(c->eye_last, c->eye, 3);
	vector_add(move, forw, move, 3);
	vector_add(move, side, move, 3);
	vector_normalize(move, 3);
	vector_add(c->eye, move, c->eye, 3);

	if (blocks_dl_should_build)
		blocks_dl_build();
}

/**
 * block_by_pos - Returns a pointer to a block by position
 * @x: X pos
 * @y: Y pos
 * @z: Z pos
 *
 * Return: Block found at Position
 */
struct block *block_by_pos(s32 x, s32 y, s32 z)
{
	if (x >= CHUNK_X || y >= CHUNK_Y || z >= CHUNK_Z)
		return (NULL);

	if (x < 0 || y < 0 || z < 0)
		return (NULL);

	return (&blocks[x][y][z]);
}

/**
 * blocks_draw - Draws everything related to the Block
 * @subtick: Delta Between Frames
 * @c: Camera
 */
void blocks_draw(f32 subtick, const struct camera *c)
{
	for (int z = 0; z < CHUNK_Z; z++)
	{
		for (int y = 0; y < CHUNK_Y; y++)
		{
			for (int x = 0; x < CHUNK_X; x++)
			{
				struct block *b = block_by_pos(x, y, z);

				b->pos[0] = x;
				b->pos[1] = y;
				b->pos[2] = z;
			}
		}
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera_view_matrix_setup(c, subtick);
	glCallList(blocks_dl);
}
