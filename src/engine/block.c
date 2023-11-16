#include <GL/gl.h>
#include <GL/glu.h>

#include "engine/util.h"
#include "engine/vertex.h"
#include "engine/block.h"
#include "engine/texture.h"
#include "engine/vector.h"

struct block blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];

static struct texture grass_side_tex;
static struct texture grass_top_tex;
static struct texture dirt_tex;

static struct vertex verts_sides[] = {
	/*
	 * front
	 */
	{{-0.5f, -0.5f,  0.5f}, {0, 1}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{ 0.5f, -0.5f,  0.5f}, {1, 1}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{-0.5f,  0.5f,  0.5f}, {0, 0}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{ 0.5f,  0.5f,  0.5f}, {1, 0}, {0xFF, 0xFF, 0xFF, 0xFF}},

	/*
	 * back
	 */
	{{-0.5f, -0.5f, -0.5f}, {1, 1}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{-0.5f,  0.5f, -0.5f}, {1, 0}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{ 0.5f, -0.5f, -0.5f}, {0, 1}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{ 0.5f,  0.5f, -0.5f}, {0, 0}, {0xFF, 0xFF, 0xFF, 0xFF}},

	/*
	 * left
	 */
	{{-0.5f, -0.5f,  0.5f}, {1, 1}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{-0.5f,  0.5f,  0.5f}, {1, 0}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{-0.5f, -0.5f, -0.5f}, {0, 1}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{-0.5f,  0.5f, -0.5f}, {0, 0}, {0xFF, 0xFF, 0xFF, 0xFF}},

	/*
	 * right
	 */
	{{ 0.5f, -0.5f,  0.5f}, {0, 1}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{ 0.5f, -0.5f, -0.5f}, {1, 1}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{ 0.5f,  0.5f,  0.5f}, {0, 0}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{ 0.5f,  0.5f, -0.5f}, {1, 0}, {0xFF, 0xFF, 0xFF, 0xFF}},
};

const struct vertex verts_top[] = {
	{{-0.5f,  0.5f,  0.5f}, {0, 1}, {0xA4, 0xFF, 0x68, 0xFF}},
	{{ 0.5f,  0.5f,  0.5f}, {0, 0}, {0xA4, 0xFF, 0x68, 0xFF}},
	{{-0.5f,  0.5f, -0.5f}, {1, 1}, {0xA4, 0xFF, 0x68, 0xFF}},
	{{ 0.5f,  0.5f, -0.5f}, {1, 0}, {0xA4, 0xFF, 0x68, 0xFF}},
};

const struct vertex verts_bottom[] = {
	{{-0.5f, -0.5f,  0.5f}, {0, 1}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{-0.5f, -0.5f, -0.5f}, {1, 1}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{ 0.5f, -0.5f,  0.5f}, {0, 0}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{ 0.5f, -0.5f, -0.5f}, {1, 0}, {0xFF, 0xFF, 0xFF, 0xFF}},
};

static u16 indis_sides[] = {
	 0,  1,  2,  2,  1,  3,
	 4,  5,  6,  6,  5,  7,
	 8,  9, 10, 10,  9, 11,
	12, 13, 14, 14, 13, 15,
};

static u16 indis_top_bottom[] = {
	0, 1, 2, 2, 1, 3
};

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

	texture_create_file(&grass_side_tex, "rom:/grass_side.ci8.sprite");
	texture_create_file(&grass_top_tex, "rom:/grass_top.i8.sprite");
	texture_create_file(&dirt_tex, "rom:/dirt.ci8.sprite");
}

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

	camera_get_focus_now(c, focus);
	vector_sub(focus, c->eye, forw, 3);
	vector3_cross(forw, (f32[3]){0, 1, 0}, side);
	vector_scale(forw, move_forw, 3);
	vector_scale(side, move_side, 3);

	vector_copy(c->eye_last, c->eye, 3);
	vector_add(move, forw, move, 3);
	vector_add(move, side, move, 3);
	vector_normalize(move, 3);
	vector_add(c->eye, move, c->eye, 3);
}

void blocks_draw(__attribute__((unused))f32 subtick, const struct camera *c)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera_view_matrix_setup(c, subtick);
	glTranslatef(0, 0, -2);

	glEnable(GL_TEXTURE_2D);

	for (int z = 0; z < CHUNK_Z; z++)
	{
		for (int y = 0; y < CHUNK_Y; y++)
		{
			for (int x = 0; x < CHUNK_X; x++)
			{
				/*
				 * draw sides
				 */
				glVertexPointer(3, GL_FLOAT,
		    			sizeof(struct vertex),
					verts_sides->pos);
				glTexCoordPointer(2, GL_FLOAT,
		      			sizeof(struct vertex),
					verts_sides->uv);
				glColorPointer(4, GL_UNSIGNED_BYTE,
		   			sizeof(struct vertex),
					verts_sides->col);

				glBindTexture(GL_TEXTURE_2D,
					grass_side_tex.id);

				glDrawElements(GL_TRIANGLES,
		   			sizeof(indis_sides) /
		   			sizeof(*indis_sides),
					GL_UNSIGNED_SHORT, indis_sides);

				/*
	                         * draw top
				 */
				glVertexPointer(3, GL_FLOAT,
		    			sizeof(struct vertex),
					verts_top->pos);
				glTexCoordPointer(2, GL_FLOAT,
		      			sizeof(struct vertex),
					verts_top->uv);
				glColorPointer(4, GL_UNSIGNED_BYTE,
		   			sizeof(struct vertex),
					verts_top->col);

				glBindTexture(GL_TEXTURE_2D, grass_top_tex.id);

				glDrawElements(GL_TRIANGLES,
		   			sizeof(indis_top_bottom) /
		   			sizeof(*indis_top_bottom),
					GL_UNSIGNED_SHORT, indis_top_bottom);

				/*
	                         * draw bottom
				 */
				glVertexPointer(3, GL_FLOAT,
		    			sizeof(struct vertex),
					verts_bottom->pos);
				glTexCoordPointer(2, GL_FLOAT,
		      			sizeof(struct vertex),
					verts_bottom->uv);
				glColorPointer(4, GL_UNSIGNED_BYTE,
		   			sizeof(struct vertex),
					verts_bottom->col);

				glBindTexture(GL_TEXTURE_2D, dirt_tex.id);

				glDrawElements(GL_TRIANGLES,
		   			sizeof(indis_top_bottom) /
		   			sizeof(*indis_top_bottom),
					GL_UNSIGNED_SHORT, indis_top_bottom);

				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}

	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
