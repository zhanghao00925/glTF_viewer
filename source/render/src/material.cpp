/*****************************/
/*  FILE NAME: material.cpp  */
/*****************************/
#include "material.h"

/* default constructor */
Material::Material()
    : alpha_mode()
    , alpha_cutoff()
    , metallic_factor()
    , roughness_factor()
    , base_color_factor()
    , emissive_factor()
    , base_color_texture_id()
    , metallic_roughness_texture_id()
    , normal_texture_id()
    , occlusion_texture_id()
    , emissive_texture_id()
    , texture_coordinate_sets()
    , extension()
    , work_flow()
{
    alpha_mode = ALPHA_MODE::ALPHA_MODE_OPAQUE;
    alpha_cutoff = 1.0f;
    metallic_factor = 1.0f;
    roughness_factor = 1.0f;
    base_color_factor = vec4(1.0f);
    emissive_factor = vec4(1.0f);
    base_color_texture_id = -1;
    metallic_roughness_texture_id = -1;
    normal_texture_id = -1;
    occlusion_texture_id = -1;
    emissive_texture_id = -1;
    work_flow = PBR_WORK_FLOW::METALLIC_ROUGHNESS;
}

/* copy constructor */
Material::Material(const Material& other)
    : alpha_mode(other.alpha_mode)
    , alpha_cutoff(alpha_cutoff)
    , metallic_factor(other.metallic_factor)
    , roughness_factor(other.roughness_factor)
    , base_color_factor(other.base_color_factor)
    , emissive_factor(other.emissive_factor)
    , base_color_texture_id(other.base_color_texture_id)
    , metallic_roughness_texture_id(other.metallic_roughness_texture_id)
    , normal_texture_id(other.normal_texture_id)
    , occlusion_texture_id(other.occlusion_texture_id)
    , emissive_texture_id(other.emissive_texture_id)
    , texture_coordinate_sets(other.texture_coordinate_sets)
    , extension(other.extension)
    , work_flow(other.work_flow)
{ /* empty */ }