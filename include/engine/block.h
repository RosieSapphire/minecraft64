#ifndef _ENGINE_BLOCK_H_
#define _ENGINE_BLOCK_H_

#include "engine/vertex.h"
#include "engine/camera.h"
#include "engine/iparms.h"

#define CHUNK_X 8
#define CHUNK_Y 8
#define CHUNK_Z 8

enum block_flags : u8
{
	BLOCK_IS_ACTIVE = 0x1,
};

struct block
{
	s32 pos[3];
	enum block_flags flags;
};

extern struct block blocks[CHUNK_X][CHUNK_Y][CHUNK_Z];
extern u32 blocks_dl;
extern u8 blocks_dl_should_build;

/*
 * Main
 */
void blocks_init(void);
void blocks_update(struct camera *c, const struct input_parms iparms);
struct block *block_by_pos(s32 x, s32 y, s32 z);
void blocks_draw(f32 subtick, const struct camera *c);

/*
 * Build
 */
u32 block_build_sides(struct vertex **vbuf, u16 **ibuf,
			const struct block *b, u32 ind);
u32 block_build_top(struct vertex **vbuf, u16 **ibuf,
			const struct block *b, u32 ind);
u32 block_build_bottom(struct vertex **vbuf, u16 **ibuf,
			const struct block *b, u32 ind);

/*
 * Displaylist
 */
void block_dl_textures_load(void);
void blocks_dl_build(void);

#endif /* _ENGINE_BLOCK_H_ */
