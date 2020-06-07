/**************************************/
/*  FILE NAME: animation_sampler.cpp  */
/**************************************/
#include "animation_sampler.h"

/* default constructor */
AnimationSampler::AnimationSampler()
    : interpolation()
    , inputs()
    , outputs()
{  
    interpolation = INTERPOLATION_TYPE::UNKNOWN;
}

/* copy constructor */
AnimationSampler::AnimationSampler(const AnimationSampler& other)
    : interpolation(other.interpolation)
    , inputs(other.inputs)
    , outputs(other.outputs)
{ /* empty */ }