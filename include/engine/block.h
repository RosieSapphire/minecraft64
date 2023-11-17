#ifndef _ENGINE_BLOCK_H_
#define _ENGINE_BLOCK_H_

#include "engine/types.h"
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
	f32 camdist;
};

extern struct block chunk[CHUNK_X][CHUNK_Y][CHUNK_Z];

void blocks_init(void);
void blocks_update(struct camera *c, const struct input_parms iparms);
void blocks_draw(__attribute__((unused))f32 subtick, const struct camera *c);

#endif /* _ENGINE_BLOCK_H_ */
