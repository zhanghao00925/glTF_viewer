/******************************/
/*  FILE NAME: extension.cpp  */
/******************************/
#include "extension.h"
#include "texture.h"
#include "shader.h"

int KHR_materials_pbrSpecularGlossiness::BindExtension(Shader shader, int slot, const map<int, Texture> &textures) const {
    int flag = 0;
    shader.setVec4("diffuse_factor", diffuse_factor);
    flag |= diffuse_texture.BindTextureInfo(shader, "diffuse", slot, textures);
    shader.setVec3("specular_factor", specular_factor);
    shader.setFloat("glossiness_factor", glossiness_factor);
    flag |= specular_glossiness_texture.BindTextureInfo(shader, "specular_glossiness", slot + 1, textures);
    return flag;
}
