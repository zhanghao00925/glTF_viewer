/****************************/
/*  FILE NAME: animation.h  */
/****************************/
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "render_core.h"
/**************/
/*  INCLUDES  */
/**************/
#include <string>
#include <vector>
#include "animation_channel.h"
#include "animation_sampler.h"

/***************************/
/*  CLASS NAME: Animation  */
/***************************/
class Animation
{
public:
    Animation();
    Animation(const Animation& other);

public:
    std::string name;
    std::vector<AnimationSampler> samplers;
    std::vector<AnimationChannel> channels;
    float start_time;
    float end_time;
}; // class Animation
#endif // !_ANIMATION_H_