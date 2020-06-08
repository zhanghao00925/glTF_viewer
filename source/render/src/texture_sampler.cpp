/************************************/
/*  FILE NAME: texture_sampler.cpp  */
/************************************/
#include "texture_sampler.h"

/* default constructor */
TextureSampler::TextureSampler()
    : name()
    , min_filter()
    , mag_filter()
    , wrap_R()
    , wrap_S()
    , wrap_T()
{ 
    min_filter = FILTER_MODE::UNKNOWN;
    mag_filter = FILTER_MODE::UNKNOWN;
    wrap_R = WRAP_MODE::UNKNOWN;
    wrap_S = WRAP_MODE::UNKNOWN;
    wrap_T = WRAP_MODE::UNKNOWN;
}

/* copy constructor */
TextureSampler::TextureSampler(const TextureSampler& other)
    : name(other.name)
    , min_filter(other.min_filter)
    , mag_filter(other.mag_filter)
    , wrap_R(other.wrap_R)
    , wrap_S(other.wrap_S)
    , wrap_T(other.wrap_T)
{ /* empty */ }

/* function to return wrap mode */
WRAP_MODE GetWrapMode(int wrap_mode)
{
    switch (wrap_mode)
    {
    case 10497: // GL_REPEAT
        return WRAP_MODE::REPEAT;
    case 33069: // GL_CLAMP_TO_BORDER
        return WRAP_MODE::CLAMP_TO_BORDER;
    case 33071: // GL_CLAMP_TO_EDGE
        return WRAP_MODE::CLAMP_TO_EDGE;
    case 33648: // GL_MIRRORED_REPEAT
        return WRAP_MODE::MIRRORED_REPEAT;
    default:
        return WRAP_MODE::UNKNOWN;
    }
}

/* function to return filter mode */
FILTER_MODE GetFilterMode(int filter_mode)
{
    switch (filter_mode)
    {
    case 9728: // NEAREST
        return FILTER_MODE::NEAREST;
    case 9729: // LINEAR
        return FILTER_MODE::LINEAR;
    case 9984: // NEAREST_MIPMAP_NEAREST
        return FILTER_MODE::NEAREST_MIPMAP_NEAREST;
    case 9985: // LINEAR_MIPMAP_NEAREST
        return FILTER_MODE::LINEAR_MIPMAP_NEAREST;
    case 9986: // NEAREST_MIPMAP_LINEAR
        return FILTER_MODE::NEAREST_MIPMAP_LINEAR;
    case 9987: // LINEAR_MIPMAP_LINEAR
        return FILTER_MODE::LINEAR_MIPMAP_LINEAR;
    default:
        return FILTER_MODE::UNKNOWN;
    }
}