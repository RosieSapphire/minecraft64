#ifndef _ENGINE_TEXTURE_H_
#define _ENGINE_TEXTURE_H_

#include <libdragon.h>

#include "engine/types.h"

/**
 * struct texture - Texture Structure
 * @id: OpenGL Texture ID
 * @spr: Libdragon Sprite Object
 */
struct texture
{
	u32 id;
	sprite_t *spr;
};

void texture_create_file(struct texture *t, const char *path);

#endif /* _ENGINE_TEXTURE_H_ */
