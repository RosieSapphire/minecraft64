#include <assert.h>
#include <GL/gl.h>

#include "engine/texture.h"

void texture_create_file(struct texture *t, const char *path)
{
	glGenTextures(1, &t->id);
	glBindTexture(GL_TEXTURE_2D, t->id);
	t->spr = sprite_load(path);
	assertf(t->spr, "Failed to load texture from '%s'\n", path);
	glSpriteTextureN64(GL_TEXTURE_2D, t->spr, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
}
