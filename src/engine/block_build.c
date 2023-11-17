#include <GL/gl.h>

#include "engine/vertex.h"
#include "engine/block.h"

u32 blocks_dl;
u8 blocks_dl_should_build = 1;

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

static u32 _block_build_face(const s32 *pos, struct vertex **v, u16 **i,
			     const struct vertex *vw,
			     const struct block *bcull, u32 ind)
{
	if (!(!bcull || (bcull && !(bcull->flags & BLOCK_IS_ACTIVE))))
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

u32 block_build_sides(struct vertex **vbuf, u16 **ibuf,
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

u32 block_build_top(struct vertex **vbuf, u16 **ibuf,
			const struct block *b, u32 ind)
{
	const s32 *p = b->pos;
	const struct block *above = block_by_pos(p[0], p[1] + 1, p[2]);

	ind = _block_build_face(p, vbuf, ibuf, verts_top, above, ind);

	return (ind);
}

u32 block_build_bottom(struct vertex **vbuf, u16 **ibuf,
			const struct block *b, u32 ind)
{
	const s32 *p = b->pos;
	const struct block *below = block_by_pos(p[0], p[1] - 1, p[2]);

	ind = _block_build_face(p, vbuf, ibuf, verts_bottom, below, ind);

	return (ind);
}
