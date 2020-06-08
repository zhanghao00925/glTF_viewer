/**********************************/
/*  FILE NAME: texture_sampler.h  */
/**********************************/
#ifndef _TEXTURE_SAMPLER_H_
#define _TEXTURE_SAMPLER_H_

/**************/
/*  INCLUDES  */
/**************/
#include <string>

/********************************/
/*  ENUM CLASS NAME: WRAP_MODE  */
/********************************/
enum class WRAP_MODE
{
    UNKNOWN,
    REPEAT,
    CLAMP_TO_BORDER,
    CLAMP_TO_EDGE,
    MIRRORED_REPEAT
}; // enum class WRAP_MODE

/**********************************/
/*  ENUM CLASS NAME: FILTER_MODE  */
/**********************************/
enum class FILTER_MODE
{
    UNKNOWN,
    LINEAR,
    LINEAR_MIPMAP_LINEAR,
    LINEAR_MIPMAP_NEAREST,
    NEAREST,
    NEAREST_MIPMAP_LINEAR,
    NEAREST_MIPMAP_NEAREST
}; // enum class FILTER_MODE

/********************************/
/*  CLASS NAME: TextureSampler  */
/********************************/
class TextureSampler
{
public:
    TextureSampler();
    TextureSampler(const TextureSampler& other);

public:
    std::string name;
    FILTER_MODE min_filter;
    FILTER_MODE mag_filter;
    WRAP_MODE wrap_R;
    WRAP_MODE wrap_S;
    WRAP_MODE wrap_T;
}; // class TextureSampler

WRAP_MODE GetWrapMode(int wrap_mode);
FILTER_MODE GetFilterMode(int filter_mode);
#endif // !_TEXTURE_SAMPLER_H_