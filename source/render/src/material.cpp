/*****************************/
/*  FILE NAME: material.cpp  */
/*****************************/
#include "material.h"

/* default constructor */
Material::Material()
    : alpha_mode()
    , alpha_cutoff()
    , emissive_factor()
    , work_flow()
{
    alpha_mode = ALPHA_MODE::ALPHA_MODE_OPAQUE;
    alpha_cutoff = 1.0f;
    emissive_factor = vec4(1.0f);
    work_flow = PBR_WORK_FLOW::METALLIC_ROUGHNESS;
}

/* copy constructor */
Material::Material(const Material& other)
    : alpha_mode(other.alpha_mode)
    , alpha_cutoff(alpha_cutoff)
    , emissive_factor(other.emissive_factor)
    , work_flow(other.work_flow)
{ /* empty */ }