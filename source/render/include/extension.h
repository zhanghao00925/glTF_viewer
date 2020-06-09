/****************************/
/*  FILE NAME: extension.h  */
/****************************/
#ifndef _EXTENSION_H_
#define _EXTENSION_H_

/**************/
/*  INCLUDES  */
/**************/
#include "tiny_gltf.h"
#include "shader.h"
#include "texture.h"
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

    int BindTextureInfo(Shader shader, const string &name, int slot, const std::map<int, Texture> &textures) const {
        int flag = 0;
        if (textures.find(index) != textures.end()) {
            shader.setInt(name + "_texture", slot);
            glActiveTexture(GL_TEXTURE0 + slot);
            textures.at(index).BindTexture();
            flag |= 1 << slot;
        }
        return flag;
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
    float scale = 1.0;    // scaledNormal = normalize((<sampled normal texture value>
    // * 2.0 - 1.0) * vec3(<normal scale>, <normal scale>, 1.0))

    NormalTextureInfo& operator=(const tinygltf::NormalTextureInfo &normalTextureInfo)
    {
        index = normalTextureInfo.index;
        texCoord = normalTextureInfo.texCoord;
        scale = normalTextureInfo.scale;
        return* this;
    }

    int BindTextureInfo(Shader shader, int slot, const std::map<int, Texture> &textures) const {
        int flag = 0;
        if (textures.find(index) != textures.end()) {
            shader.setInt("normal_texture", slot);
            glActiveTexture(GL_TEXTURE0 + slot);
            textures.at(index).BindTexture();
            shader.setFloat("normal_scale", scale);
            flag |= 1 << slot;
        }
        return flag;
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
    float strength = 1.0;  // occludedColor = lerp(color, color * <sampled occlusion
    // texture value>, <occlusion strength>)

    OcclusionTextureInfo& operator=(const tinygltf::OcclusionTextureInfo &occlusionTextureInfo)
    {
        index = occlusionTextureInfo.index;
        texCoord = occlusionTextureInfo.texCoord;
        strength = occlusionTextureInfo.strength;
        return* this;
    }

    int  BindTextureInfo(Shader shader, int slot, const std::map<int, Texture> &textures) const {
        int flag = 0;
        if (textures.find(index) != textures.end()) {
            shader.setInt("occlusion_texture", slot);
            glActiveTexture(GL_TEXTURE0 + slot);
            textures.at(index).BindTexture();
            shader.setFloat("occlusion_strength", strength);
            flag |= 1 << slot;
        }
        return flag;
    }
};

/************************************/
/* CLASS NAME: PbrMetallicRoughness */
/************************************/
// pbrMetallicRoughness class defined in glTF 2.0 spec.
class PbrMetallicRoughness {
public:
    vec4 baseColorFactor = vec4(1.0f);  // len = 4. default [1,1,1,1]
    TextureInfo baseColorTexture;
    float metallicFactor = 1.0f;   // default 1
    float roughnessFactor = 1.0f;  // default 1
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

    // return next availiable texture slot
    int BindTextureInfo(Shader shader, int slot, const std::map<int, Texture> &textures) const {
        // pbr Metallic roughness
        shader.setFloat("metallic_factor", metallicFactor);
        shader.setFloat("roughness_factor", roughnessFactor);
        shader.setVec4("baseColor_factor", baseColorFactor);
        // pbr baseColor Texture
        int flag = 0;
        flag |= baseColorTexture.BindTextureInfo(shader, "baseColor", slot, textures);
        flag |= baseColorTexture.BindTextureInfo(shader, "metallicRoughness", slot + 1, textures);
        return flag;
    }
};

/***************************/
/*  CLASS NAME: Extension  */
/***************************/
class Extension
{
public:
    virtual int BindExtension(class Shader shader, int slot, const std::map<int, class Texture> &textures) const  = 0;
public:
}; // class Extension


/***************************/
/*  CLASS NAME: Extension  */
/***************************/
class KHR_materials_pbrSpecularGlossiness : public Extension
{
public:
    int BindExtension(Shader shader, int slot, const std::map<int, Texture> &textures) const override;
public:
    TextureInfo diffuse_texture;
    vec4 diffuse_factor = vec4(1.0f);
    vec3 specular_factor = vec3(1.0f);
    float glossiness_factor = 1.0f;
    TextureInfo specular_glossiness_texture;
}; // class Extension

#endif // !_EXTENSION_H_