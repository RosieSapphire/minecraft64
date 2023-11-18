#include <assert.h>
#include <GL/gl.h>

#include "engine/texture.h"

/**
 * texture_create_file - Creates Texture from File
 * @t: Texture to Write to
 * @path: Path to Read from
 */
void texture_create_file(struct texture *t, const char *path)
{
	glGenTextures(1, &t->id);
	glBindTexture(GL_TEXTURE_2D, t->id);
	t->spr = sprite_load(path);
	assertf(t->spr, "Failed to load texture from '%s'\n", path);
	glSpriteTextureN64(GL_TEXTURE_2D, t->spr, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}
