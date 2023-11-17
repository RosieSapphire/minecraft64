#include <GL/gl.h>

#include "engine/texture.h"
#include "engine/vertex.h"
#include "engine/block.h"

u32 blocks_dl;
u8 blocks_dl_should_build = 1;

static const u32 face_size = sizeof(struct vertex) * 4;

static struct texture grass_side_tex;
static struct texture grass_top_tex;
static struct texture dirt_tex;

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

void block_textures_load(void)
{
	texture_create_file(&grass_side_tex, "rom:/grass_side.ci8.sprite");
	texture_create_file(&grass_top_tex, "rom:/grass_top.i8.sprite");
	texture_create_file(&dirt_tex, "rom:/dirt.ci8.sprite");
}

static u32 _block_build_face(const s32 *pos, struct vertex **v, u16 **i,
			     const struct vertex *vw,
			     const struct block *bcull, u32 ind)
{
	if(!(!bcull || (bcull && !(bcull->flags & BLOCK_IS_ACTIVE))))
		return (ind);

	struct vertex vbase[4];
	u16 ibase[6] = {0, 1, 2, 2, 1, 3};

	memcpy(vbase, vw, face_size);

	for (u16 i = 0; i < 4; i++)
		for (u16 j = 0; j < 3; j++)
			vbase[i].pos[j] += pos[j];

	for (u16 i = 0; i < 6; i++)
		ibase[i] += 4 * ind;

	*v = realloc(*v, face_size * (ind + 1));
	*i = realloc(*i, sizeof(u16) * 6 * (ind + 1));
	memcpy(*v + (ind * 4), vbase, face_size);
	memcpy(*i + (ind * 6), ibase, sizeof(u16) * 6);
	ind++;

	return (ind);

}

static u32 _block_build_sides(struct vertex **vbuf, u16 **ibuf,
			const struct block *b, u32 ind)
{
	const s32 *p = b->pos;
	const struct block *infront = block_by_pos(p[0], p[1], p[2] + 1);
	const struct block *behind = block_by_pos(p[0], p[1], p[2] - 1);
	const struct block *toleft = block_by_pos(p[0] - 1, p[1], p[2]);
	const struct block *toright = block_by_pos(p[0] + 1, p[1], p[2]);

	ind = _block_build_face(p, vbuf, ibuf, verts_front, infront, ind);
	ind = _block_build_face(p, vbuf, ibuf, verts_back, behind, ind);
	ind = _block_build_face(p, vbuf, ibuf, verts_left, toleft, ind);
	ind = _block_build_face(p, vbuf, ibuf, verts_right, toright, ind);

	return (ind);
}

static u32 _block_build_top(struct vertex **vbuf, u16 **ibuf,
			const struct block *b, u32 ind)
{
	const s32 *p = b->pos;
	const struct block *above = block_by_pos(p[0], p[1] + 1, p[2]);

	ind = _block_build_face(p, vbuf, ibuf, verts_top, above, ind);

	return (ind);
}

static u32 _block_build_bottom(struct vertex **vbuf, u16 **ibuf,
			const struct block *b, u32 ind)
{
	const s32 *p = b->pos;
	const struct block *below = block_by_pos(p[0], p[1] - 1, p[2]);

	ind = _block_build_face(p, vbuf, ibuf, verts_bottom, below, ind);

	return (ind);
}

void blocks_dl_build(void)
{
	struct vertex *vertbuff_sides = malloc(0);
	struct vertex *vertbuff_top = malloc(0);
	struct vertex *vertbuff_bottom = malloc(0);
	u16 *indibuff_sides = malloc(0);
	u16 *indibuff_top = malloc(0);
	u16 *indibuff_bottom = malloc(0);
	u32 num_faces_sides = 0, num_faces_top = 0, num_faces_bottom = 0;

	for (int i = 0; i < CHUNK_Z * CHUNK_X * CHUNK_Y; i++)
	{
		const struct block *b = (const struct block *)blocks + i;

		num_faces_sides = _block_build_sides(&vertbuff_sides,
				&indibuff_sides, b, num_faces_sides);
		num_faces_top = _block_build_top(&vertbuff_top,
				&indibuff_top, b, num_faces_top);
		num_faces_bottom = _block_build_bottom(&vertbuff_bottom,
				&indibuff_bottom, b, num_faces_bottom);
	}

	blocks_dl = glGenLists(1);
	glNewList(blocks_dl, GL_COMPILE);

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	/*
	 * Sides
	 */
	glVertexPointer(3, GL_FLOAT,
		 sizeof(struct vertex), vertbuff_sides->pos);
	glTexCoordPointer(2, GL_FLOAT,
		   sizeof(struct vertex), vertbuff_sides->uv);
	glColorPointer(4, GL_UNSIGNED_BYTE,
		sizeof(struct vertex), vertbuff_sides->col);

	glBindTexture(GL_TEXTURE_2D, grass_side_tex.id);
	glDrawElements(GL_TRIANGLES, num_faces_sides * 6,
		GL_UNSIGNED_SHORT, indibuff_sides);

	/*
	 * Top
	 */
	glVertexPointer(3, GL_FLOAT,
		 sizeof(struct vertex), vertbuff_top->pos);
	glTexCoordPointer(2, GL_FLOAT,
		   sizeof(struct vertex), vertbuff_top->uv);
	glColorPointer(4, GL_UNSIGNED_BYTE,
		sizeof(struct vertex), vertbuff_top->col);

	glBindTexture(GL_TEXTURE_2D, grass_top_tex.id);
	glDrawElements(GL_TRIANGLES, num_faces_top * 6,
		GL_UNSIGNED_SHORT, indibuff_top);

	/*
	 * Bottom
	 */
	glVertexPointer(3, GL_FLOAT,
		 sizeof(struct vertex), vertbuff_bottom->pos);
	glTexCoordPointer(2, GL_FLOAT,
		   sizeof(struct vertex), vertbuff_bottom->uv);
	glColorPointer(4, GL_UNSIGNED_BYTE,
		sizeof(struct vertex), vertbuff_bottom->col);

	glBindTexture(GL_TEXTURE_2D, dirt_tex.id);
	glDrawElements(GL_TRIANGLES, num_faces_bottom * 6,
		GL_UNSIGNED_SHORT, indibuff_bottom);

	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEndList();

	free(vertbuff_sides);
	free(vertbuff_top);
	free(vertbuff_bottom);
	free(indibuff_sides);
	free(indibuff_top);
	free(indibuff_bottom);

	blocks_dl_should_build = 0;
}
