/*****************************/
/*  FILE NAME: material.cpp  */
/*****************************/
#include <texture.h>
#include "material.h"
#include "shader.h"

/* default constructor */
Material::Material()
{
    alpha_mode = ALPHA_MODE::ALPHA_MODE_OPAQUE;
    work_flow = PBR_WORK_FLOW::METALLIC_ROUGHNESS;
}


void Material::BindMaterial(Shader shader, const std::map<int, Texture> &textures) const {
    if (double_sided) {
        glEnable(GL_CULL_FACE);
    } else {
        glDisable(GL_CULL_FACE);
    }
    shader.setInt("alpha_mode", (int)alpha_mode);
    if (alpha_mode == ALPHA_MODE::ALPHA_MODE_BLEND) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_BLEND);
    }
    shader.setInt("work_flow", (int)work_flow);
    shader.setFloat("alpha_cutoff", alpha_cutoff);
    shader.setVec3("emissive_factor", emissive_factor);
    int flag = 0;
    flag |= emissiveTexture.BindTextureInfo(shader, "emissive", 0, textures);
    flag |= normalTexture.BindTextureInfo(shader, 1, textures);
    flag |= occlusionTexture.BindTextureInfo(shader, 2, textures);
    flag |= pbrMetallicRoughness.BindTextureInfo(shader, 3, textures);

    // bind extension
    for (auto &extension : extensions) {
        flag |= extension->BindExtension(shader, 5, textures);
    }
    shader.setInt("flag", flag);
}
