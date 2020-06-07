/**************************************/
/*  FILE NAME: animation_channel.cpp  */
/**************************************/
#include "animation_channel.h"

/* default constructor */
AnimationChannel::AnimationChannel()
    : path_type()
    , node_id()
    , sampler_id()
{
    path_type = PATH_TYPE::UNKNOWN;
    node_id = -1;
    sampler_id = -1;
}

/* copy constructor */
AnimationChannel::AnimationChannel(const AnimationChannel& other)
    : path_type(other.path_type)
    , node_id(other.node_id)
    , sampler_id(other.sampler_id)
{ /* empty */ }