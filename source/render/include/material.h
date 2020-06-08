/***************************/
/*  FILE NAME: material.h  */
/***************************/
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

/**************/
/*  INCLUDES  */
/**************/
#include "texture_coordinate_sets.h"
#include "extension.h"

/*********************************/
/*  ENUM CLASS NAME: ALPHA_MODE  */
/*********************************/
enum class ALPHA_MODE
{
    ALPHA_MODE_OPAQUE,
    ALPHA_MODE_MASK,
    ALPHA_MODE_BLEND
}; // enum class ALPHA_MODE

/************************************/
/*  ENUM CLASS NAME: PBR_WORK_FLOW  */
/************************************/
enum class PBR_WORK_FLOW
{
    METALLIC_ROUGHNESS,
    SPECULAR_GLOSSINESS
}; // enum class PBR_WORK_FLOW

/**************************/
/*  CLASS NAME: Material  */
/**************************/
class Material
{
public:
    Material();
    Material(const Material& other);

public:
    ALPHA_MODE alpha_mode;

    float alpha_cutoff;
    float metallic_factor;
    float roughness_factor;
    vec4 base_color_factor;
    vec4 emissive_factor;

    int base_color_texture_id;
    int metallic_roughness_texture_id;
    int normal_texture_id;
    int occlusion_texture_id;
    int emissive_texture_id;

    TextureCoordinateSets texture_coordinate_sets;
    Extension extension;

    PBR_WORK_FLOW work_flow;
}; // class Material
#endif // !_MATERIAL_H_