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
enum class ALPHA_MODE : int
{
    ALPHA_MODE_OPAQUE = 0,
    ALPHA_MODE_MASK,
    ALPHA_MODE_BLEND
}; // enum class ALPHA_MODE

/************************************/
/*  ENUM CLASS NAME: PBR_WORK_FLOW  */
/************************************/
enum class PBR_WORK_FLOW : int
{
    METALLIC_ROUGHNESS = 0,
    SPECULAR_GLOSSINESS
}; // enum class PBR_WORK_FLOW

/**************************/
/*  CLASS NAME: Material  */
/**************************/
class Material
{
public:
    Material();

    void BindMaterial(class Shader shader, const std::map<int, Texture> &textures) const;

public:
    vec3 emissive_factor = vec3(1.0f);
    ALPHA_MODE alpha_mode;
    float alpha_cutoff = 1.0f;
    bool double_sided = true;
    PbrMetallicRoughness pbrMetallicRoughness;
    NormalTextureInfo normalTexture;
    OcclusionTextureInfo occlusionTexture;
    TextureInfo emissiveTexture;

    vector<shared_ptr<Extension>> extensions;

    PBR_WORK_FLOW work_flow;
}; // class Material
#endif // !_MATERIAL_H_