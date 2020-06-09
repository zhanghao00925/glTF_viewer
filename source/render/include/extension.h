/****************************/
/*  FILE NAME: extension.h  */
/****************************/
#ifndef _EXTENSION_H_
#define _EXTENSION_H_

/**************/
/*  INCLUDES  */
/**************/
#include "tiny_gltf.h"
#include "render_core.h"

/************************************/
/*     CLASS NAME: TextureInfo      */
/************************************/
class TextureInfo {
public:
    int index = -1;  // required.
    int texCoord = 0;    // The set index of texture's TEXCOORD attribute used for
    // texture coordinate mapping.

    TextureInfo& operator=(const tinygltf::TextureInfo &textureInfo)
    {
        index = textureInfo.index;
        texCoord = textureInfo.texCoord;
        return* this;
    }
};

/************************************/
/*  CLASS NAME: NormalTextureInfo   */
/************************************/
class NormalTextureInfo {
public:
    int index = -1;  // required
    int texCoord = 0;    // The set index of texture's TEXCOORD attribute used for
    // texture coordinate mapping.
    double scale = 1.0;    // scaledNormal = normalize((<sampled normal texture value>
    // * 2.0 - 1.0) * vec3(<normal scale>, <normal scale>, 1.0))

    NormalTextureInfo& operator=(const tinygltf::NormalTextureInfo &normalTextureInfo)
    {
        index = normalTextureInfo.index;
        texCoord = normalTextureInfo.texCoord;
        scale = normalTextureInfo.scale;
        return* this;
    }
};

/************************************/
/* CLASS NAME: OcclusionTextureInfo */
/************************************/
class OcclusionTextureInfo {
public:
    int index = -1;   // required
    int texCoord = 0;     // The set index of texture's TEXCOORD attribute used for
    // texture coordinate mapping.
    double strength = 1.0;  // occludedColor = lerp(color, color * <sampled occlusion
    // texture value>, <occlusion strength>)

    OcclusionTextureInfo& operator=(const tinygltf::OcclusionTextureInfo &occlusionTextureInfo)
    {
        index = occlusionTextureInfo.index;
        texCoord = occlusionTextureInfo.texCoord;
        strength = occlusionTextureInfo.strength;
        return* this;
    }
};

/************************************/
/* CLASS NAME: PbrMetallicRoughness */
/************************************/
// pbrMetallicRoughness class defined in glTF 2.0 spec.
struct PbrMetallicRoughness {
    vec4 baseColorFactor = vec4(1.0f);  // len = 4. default [1,1,1,1]
    TextureInfo baseColorTexture;
    double metallicFactor = 1.0f;   // default 1
    double roughnessFactor = 1.0f;  // default 1
    TextureInfo metallicRoughnessTexture;

    PbrMetallicRoughness& operator=(const tinygltf::PbrMetallicRoughness &pbrMetallicRoughness)
    {
        baseColorFactor = make_vec4(pbrMetallicRoughness.baseColorFactor.data());
        baseColorTexture = pbrMetallicRoughness.baseColorTexture;
        metallicFactor = pbrMetallicRoughness.metallicFactor;
        roughnessFactor = pbrMetallicRoughness.roughnessFactor;
        metallicRoughnessTexture = pbrMetallicRoughness.metallicRoughnessTexture;
        return* this;
    }
};

/***************************/
/*  CLASS NAME: Extension  */
/***************************/
class Extension
{
public:
    virtual void BindExtension() = 0;
public:
}; // class Extension


/***************************/
/*  CLASS NAME: Extension  */
/***************************/
class KHR_materials_pbrSpecularGlossiness : public Extension
{
public:
    void BindExtension() override {}
public:
    TextureInfo diffuse_texture;
    vec4 diffuse_factor = vec4(1.0f);
    vec3 specular_factor = vec3(1.0f);
    float glossinessFactor = 1.0f;
    TextureInfo specular_glossiness_texture;
}; // class Extension

#endif // !_EXTENSION_H_