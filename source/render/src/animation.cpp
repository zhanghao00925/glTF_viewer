/******************************/
/*  FILE NAME: animation.cpp  */
/******************************/
#include "animation.h"

/**************/
/*  INCLUDES  */
/**************/
#include <limits>

/* default constructor */
Animation::Animation()
    : name()
    , samplers()
    , channels()
    , start_time()
    , end_time()
{
    start_time = std::numeric_limits<float>::max();
    end_time = std::numeric_limits<float>::min();
}

/* copy constructor */
Animation::Animation(const Animation& other)
    : name(other.name)
    , samplers(other.samplers)
    , channels(other.channels)
    , start_time(other.start_time)
    , end_time(other.end_time)
{ /* empty */ }