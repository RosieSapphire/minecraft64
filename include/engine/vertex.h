#ifndef _ENGINE_VERTEX_H_
#define _ENGINE_VERTEX_H_

#include "engine/types.h"

/**
 * struct vertex - Vertex Structure
 * @pos: Position Component
 * @uv: UV Component
 * @col: Color Component
 */
struct vertex
{
	f32 pos[3];
	f32 uv[2];
	u8 col[4];
};

#endif /* _ENGINE_VERTEX_H_ */
