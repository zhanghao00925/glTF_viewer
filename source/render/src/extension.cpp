/******************************/
/*  FILE NAME: extension.cpp  */
/******************************/
#include "extension.h"

/* default constructor */
Extension::Extension()
    : specular_glossiness_texture_id()
    , diffuse_texture_id()
    , diffuse_factor()
    , specular_factor()
{
    specular_glossiness_texture_id = -1;
    diffuse_texture_id = -1;
    diffuse_factor = vec4(1.0f);
    specular_factor = vec3(1.0f);
}

/* copy constructor */
Extension::Extension(const Extension& other)
    : specular_glossiness_texture_id(other.specular_glossiness_texture_id)
    , diffuse_texture_id(other.diffuse_texture_id)
    , diffuse_factor(other.diffuse_factor)
    , specular_factor(other.specular_factor)
{ /* empty */ }