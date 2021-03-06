#version 440 core

#define MAX_MORPHS 3
#define MAX_BONES 120

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;
layout (location = 3) in uvec4 joint;
layout (location = 4) in vec4 weight;
layout (location = 5) in vec3 position_displace[MAX_MORPHS];
layout (location = 8) in vec3 normal_displace[MAX_MORPHS];

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 projection;
uniform mat4 view;

uniform int bSkin;
uniform int num_morphs;

uniform mat4 local_model;
uniform float morph_weights[MAX_MORPHS];

uniform mat4 bone_matrix[MAX_BONES];

void main()
{
    mat4 mesh_matrix;
    if (bSkin == 1) {
        mat4 bond_transform = bone_matrix[joint[0]] * weight[0];
        bond_transform += bone_matrix[joint[1]] * weight[1];
        bond_transform += bone_matrix[joint[2]] * weight[2];
        bond_transform += bone_matrix[joint[3]] * weight[3];
        mesh_matrix = local_model * bond_transform;
        
    } else {
        mesh_matrix = local_model;
    }
    vec3 final_position = position;
    vec3 final_normal = normal;
    for (int i = 0; i < MAX_MORPHS && i < num_morphs; i++) {
        final_position += position_displace[i] * morph_weights[i];
        final_normal += normal_displace[i] * morph_weights[i];
    }
    WorldPos = (mesh_matrix * vec4(final_position, 1.0)).xyz;
    gl_Position  = projection * view * mesh_matrix * vec4(final_position, 1.0);
    Normal       = mat3(transpose(inverse(mesh_matrix))) * final_normal;
    TexCoords     = texcoord;

}
