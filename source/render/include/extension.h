/****************************/
/*  FILE NAME: extension.h  */
/****************************/
#ifndef _EXTENSION_H_
#define _EXTENSION_H_

/**************/
/*  INCLUDES  */
/**************/
#include "render_core.h"

/***************************/
/*  CLASS NAME: Extension  */
/***************************/
class Extension
{
public:
    Extension();
    Extension(const Extension& other);

public:
    int specular_glossiness_texture_id;
    int diffuse_texture_id;
    vec4 diffuse_factor;
    vec3 specular_factor;
}; // class Extension
#endif // !_EXTENSION_H_