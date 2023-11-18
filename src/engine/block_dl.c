#include <GL/gl.h>

#include "engine/vertex.h"
#include "engine/texture.h"
#include "engine/block.h"

static struct texture grass_side_tex;
static struct texture grass_top_tex;
static struct texture dirt_tex;

/**
 * block_dl_textures_load - Loads the Block Textures
 */
void block_dl_textures_load(void)
{
	texture_create_file(&grass_side_tex, "rom:/grass_side.ci8.sprite");
	texture_create_file(&grass_top_tex, "rom:/grass_top.i8.sprite");
	texture_create_file(&dirt_tex, "rom:/dirt.ci8.sprite");
}

/**
 * _blocks_dl_draw_part - Draws a Consolidated Part of a Chunk
 * @vb: Vertex Buffer
 * @ib: Index Buffer
 * @num_faces: Number of Faces
 * @tid: OpenGL Texture ID
 */
static void _blocks_dl_draw_part(const struct vertex *vb, const u16 *ib,
			       const u16 num_faces, const u32 tid)
{
	glVertexPointer(3, GL_FLOAT, sizeof(struct vertex), vb->pos);
	glTexCoordPointer(2, GL_FLOAT, sizeof(struct vertex), vb->uv);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(struct vertex), vb->col);
	glBindTexture(GL_TEXTURE_2D, tid);
	glDrawElements(GL_TRIANGLES, num_faces * 6, GL_UNSIGNED_SHORT, ib);
}

/**
 * _blocks_dl_gen_gllist - Generates a Display List from Mesh Data
 * @vbs: Vertex Buffer Sides
 * @vbt: Vertex Buffer Top
 * @vbb: Vertex Buffer Bottom
 * @ibs: Index Buffer Sides
 * @ibt: Index Buffer Top
 * @ibb: Index Buffer Bottom
 * @nfs: Number of Faces Sides
 * @nft: Number of Faces Top
 * @nfb: Number of Faces Bottom
 * @ts: Texture Sides
 * @tt: Texture Top
 * @tb: Texture Bottom
 */
static void _blocks_dl_gen_gllist(struct vertex *vbs, struct vertex *vbt,
				  struct vertex *vbb, u16 *ibs,
				  u16 *ibt, u16 *ibb, u32 nfs, u32 nft,
				  u32 nfb, u32 ts, u32 tt, u32 tb)
{
	blocks_dl = glGenLists(1);
	glNewList(blocks_dl, GL_COMPILE);

	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	_blocks_dl_draw_part(vbs, ibs, nfs, ts);
	_blocks_dl_draw_part(vbt, ibt, nft, tt);
	_blocks_dl_draw_part(vbb, ibb, nfb, tb);

	glDisable(GL_TEXTURE_2D);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glEndList();
}

/**
 * blocks_dl_build - Builds the Display List for the entire Chunk
 */
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

		num_faces_sides = block_build_sides(&vertbuff_sides,
				&indibuff_sides, b, num_faces_sides);
		num_faces_top = block_build_top(&vertbuff_top,
				&indibuff_top, b, num_faces_top);
		num_faces_bottom = block_build_bottom(&vertbuff_bottom,
				&indibuff_bottom, b, num_faces_bottom);
	}

	_blocks_dl_gen_gllist(vertbuff_sides, vertbuff_top, vertbuff_bottom,
				indibuff_sides, indibuff_top, indibuff_bottom,
				num_faces_sides, num_faces_top,
				num_faces_bottom, grass_side_tex.id,
				grass_top_tex.id, dirt_tex.id);

	free(vertbuff_sides);
	free(vertbuff_top);
	free(vertbuff_bottom);
	free(indibuff_sides);
	free(indibuff_top);
	free(indibuff_bottom);

	blocks_dl_should_build = 0;
}
