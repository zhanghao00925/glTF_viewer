/************************/
/* FILE NAME: vertex.h  */
/************************/
#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "render_core.h"
/**************/
/*  INCLUDES  */
/**************/

/************************/
/*  CLASS NAME: Vertex  */
/************************/
class GLTFVertex
{
public:
    GLTFVertex();
    GLTFVertex(const GLTFVertex& other);

public:
    vec3 position;
    vec3 normal;
    vec2 texcoord;
    uvec4 joint;
    vec4 weight;
}; // class Vertex
#endif // !_VERTEX_H_