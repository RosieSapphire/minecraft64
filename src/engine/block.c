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

static const u32 face_size = sizeof(struct vertex) * 4;

static struct vertex verts_front[] = {
	{{-0.5f, -0.5f,  0.5f}, {0, 1}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{ 0.5f, -0.5f,  0.5f}, {1, 1}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{-0.5f,  0.5f,  0.5f}, {0, 0}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{ 0.5f,  0.5f,  0.5f}, {1, 0}, {0xFF, 0xFF, 0xFF, 0xFF}},
};

static struct vertex verts_back[] = {
	{{-0.5f, -0.5f, -0.5f}, {1, 1}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{-0.5f,  0.5f, -0.5f}, {1, 0}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{ 0.5f, -0.5f, -0.5f}, {0, 1}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{ 0.5f,  0.5f, -0.5f}, {0, 0}, {0xFF, 0xFF, 0xFF, 0xFF}},
};

static struct vertex verts_left[] = {
	{{-0.5f, -0.5f,  0.5f}, {1, 1}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{-0.5f,  0.5f,  0.5f}, {1, 0}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{-0.5f, -0.5f, -0.5f}, {0, 1}, {0xFF, 0xFF, 0xFF, 0xFF}},
	{{-0.5f,  0.5f, -0.5f}, {0, 0}, {0xFF, 0xFF, 0xFF, 0xFF}},
};

static struct vertex verts_right[] = {
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
	const u8 go_fast = iparms.held.z + 1;

	camera_get_focus_now(c, focus);
	vector_sub(focus, c->eye, forw, 3);
	vector3_cross(forw, (f32[3]){0, 1, 0}, side);
	vector_scale(forw, move_forw * go_fast, 3);
	vector_scale(side, move_side * go_fast, 3);

	vector_copy(c->eye_last, c->eye, 3);
	vector_add(move, forw, move, 3);
	vector_add(move, side, move, 3);
	vector_normalize(move, 3);
	vector_add(c->eye, move, c->eye, 3);

	/*
	 * sorting blocks
	 */
}

/*
static int _block_sort_func(const void *a, const void *b)
{
	const struct block *ab = (const struct block *)a;
	const struct block *bb = (const struct block *)b;

	return (ab->camdist - bb->camdist);
}
*/

static struct block *_block_by_pos(s32 x, s32 y, s32 z)
{
	if (x >= CHUNK_X || y >= CHUNK_Y || z >= CHUNK_Z)
		return (NULL);

	if (x < 0 || y < 0 || z < 0)
		return (NULL);

	return (&blocks[x][y][z]);
}

static void _block_draw_face(const s32 *pos, struct vertex **v, u16 **i,
			     const struct vertex *vw,
			     const struct block *bcull, u32 *ind)
{
	if(!(!bcull || (bcull && !(bcull->flags & BLOCK_IS_ACTIVE))))
		return;

	struct vertex vbase[4];
	u16 ibase[6] = {0, 1, 2, 2, 1, 3};

	memcpy(vbase, vw, face_size);

	for (u16 i = 0; i < 4; i++)
		for (u16 j = 0; j < 3; j++)
			vbase[i].pos[j] += pos[j];

	for (u16 i = 0; i < 6; i++)
		ibase[i] += 4 * (*ind);

	*v = realloc(*v, face_size * (*ind + 1));
	*i = realloc(*i, sizeof(u16) * 6 * (*ind + 1));
	memcpy(*v + (*ind * 4), vbase, face_size);
	memcpy(*i + (*ind * 6), ibase, sizeof(u16) * 6);
	*ind += 1;

}

static void _block_draw(struct vertex **vbuf, u16 **ibuf,
			const struct block *b, u32 *face_ind)
{
	const s32 *p = b->pos;
	const struct block *infront = _block_by_pos(p[0], p[1], p[2] + 1);
	const struct block *behind = _block_by_pos(p[0], p[1], p[2] - 1);
	const struct block *toleft = _block_by_pos(p[0] - 1, p[1], p[2]);
	const struct block *toright = _block_by_pos(p[0] + 1, p[1], p[2]);
	const struct block *above = _block_by_pos(p[0], p[1] + 1, p[2]);
	const struct block *below = _block_by_pos(p[0], p[1] - 1, p[2]);

	/*
	glPushMatrix();
	glTranslatef(p[0], p[1], p[2]);
	*/

	_block_draw_face(p, vbuf, ibuf, verts_front, infront, face_ind);
	_block_draw_face(p, vbuf, ibuf, verts_back, behind, face_ind);
	_block_draw_face(p, vbuf, ibuf, verts_left, toleft, face_ind);
	_block_draw_face(p, vbuf, ibuf, verts_right, toright, face_ind);
	_block_draw_face(p, vbuf, ibuf, verts_top, above, face_ind);
	_block_draw_face(p, vbuf, ibuf, verts_bottom, below, face_ind);

	/*
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	*/
}

void blocks_draw(__attribute__((unused))f32 subtick, const struct camera *c)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera_view_matrix_setup(c, subtick);

	glEnable(GL_TEXTURE_2D);

	for (int z = 0; z < CHUNK_Z; z++)
	{
		for (int y = 0; y < CHUNK_Y; y++)
		{
			for (int x = 0; x < CHUNK_X; x++)
			{
				struct block *b = &blocks[x][y][z];
				f32 camvec[3];

				vector_sub(c->eye,
	       				(f32[3]){x, y, z}, camvec, 3);
				b->camdist = vector_magnitude_sqr(camvec, 3);
				b->pos[0] = x;
				b->pos[1] = y;
				b->pos[2] = z;
			}
		}
	}

	/*
	const u32 blocks_size =
		sizeof(struct block) * CHUNK_X * CHUNK_Y * CHUNK_Z;
	qsort(blocks_sorted, CHUNK_X * CHUNK_Y * CHUNK_Z,
       		sizeof(struct block), _block_sort_func);
	*/

	struct vertex *vertbuff = malloc(0);
	u16 *indibuff = malloc(0);
	u32 num_faces = 0;

	for (int i = 0; i < CHUNK_Z * CHUNK_X * CHUNK_Y; i++)
	{
		const struct block *b = (const struct block *)blocks + i;

		_block_draw(&vertbuff, &indibuff, b, &num_faces);
	}

	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex), vertbuff->pos);
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex), vertbuff->uv);
	glColorPointer(4, GL_UNSIGNED_BYTE,
		sizeof(struct vertex), vertbuff->col);
	
	glBindTexture(GL_TEXTURE_2D, grass_side_tex.id);
	glDrawElements(GL_TRIANGLES, num_faces * 6,
		GL_UNSIGNED_SHORT, indibuff);

	free(vertbuff);
	free(indibuff);

	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
