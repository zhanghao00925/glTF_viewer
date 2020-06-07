/************************************/
/*  FILE NAME: animation_channel.h  */
/************************************/
#ifndef _ANIMATION_CHANNEL_H_
#define _ANIMATION_CHANNEL_H_

#include "render_core.h"
/********************************/
/*  ENUM CLASS NAME: PATH_TYPE  */
/********************************/
enum class PATH_TYPE
{
    UNKNOWN,
    TRANSLATION,
    ROTATION,
    SCALE,
    WEIGHTS
}; // enum class PATH_TYPE

/**********************************/
/*  CLASS NAME: AnimationChannel  */
/**********************************/
class AnimationChannel
{
public:
    AnimationChannel();
    AnimationChannel(const AnimationChannel& other);

public:
    PATH_TYPE path_type;
    int node_id;
    int sampler_id;
}; // class AnimationChannel
#endif // !_ANIMATION_CHANNEL_H_  