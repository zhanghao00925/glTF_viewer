/***************************/
/*  FILE NAME: material.h  */
/***************************/
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

/**************/
/*  INCLUDES  */
/**************/
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

    void BindMaterial();

public:
    vec3 emissive_factor;
    ALPHA_MODE alpha_mode;
    float alpha_cutoff;
    bool double_sided;
    PbrMetallicRoughness pbrMetallicRoughness;
    NormalTextureInfo normalTexture;
    OcclusionTextureInfo occlusionTexture;
    TextureInfo emissiveTexture;

    vector<shared_ptr<Extension>> extensions;

    PBR_WORK_FLOW work_flow;
}; // class Material
#endif // !_MATERIAL_H_