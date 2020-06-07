#version 440 core

#define MAX_MORPHS 3
#define MAX_BONES 120

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;
layout (location = 3) in uvec4 joint;
layout (location = 4) in vec4 weight;
layout (location = 5) in vec3 position_displace[MAX_MORPHS];
layout (location = 8) in vec3 pre_position_displace[MAX_MORPHS];
// layout (location = 10) in vec3 normal_displace[MAX_MORPHS];

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

out vec4 ss_pos;
out vec4 pre_ss_pos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 pre_view;

uniform int bSkin;

uniform mat4 local_model;
uniform mat4 pre_local_model;
uniform float morph_weights[MAX_MORPHS];
uniform float pre_morph_weights[MAX_MORPHS];

uniform mat4 bone_matrix[MAX_BONES];
uniform mat4 pre_bone_matrix[MAX_BONES];

void main()
{

    if (bSkin == 1) {
        mat4 bond_transform = bone_matrix[joint[0]] * weight[0];
        bond_transform += bone_matrix[joint[1]] * weight[1];
        bond_transform += bone_matrix[joint[2]] * weight[2];
        bond_transform += bone_matrix[joint[3]] * weight[3];

        mat4 pre_bond_transform = pre_bone_matrix[joint[0]] * weight[0];
        pre_bond_transform += pre_bone_matrix[joint[1]] * weight[1];
        pre_bond_transform += pre_bone_matrix[joint[2]] * weight[2];
        pre_bond_transform += pre_bone_matrix[joint[3]] * weight[3];

        WorldPos = (bond_transform * vec4(position, 1.0)).xyz;
        gl_Position  = projection * view * bond_transform * vec4(position, 1.0);
        Normal       = mat3(transpose(inverse(bond_transform))) * normal;

        ss_pos = projection * view * bond_transform * vec4(position, 1.0);
        pre_ss_pos = projection * pre_view * pre_bond_transform * vec4(position, 1.0);
    } else {
        vec3 final_position = position;
        vec3 pre_final_position = position;
        vec3 final_normal = normal;
        for (int i = 0; i < MAX_MORPHS; i++) {
            final_position += position_displace[i] * morph_weights[i];
            pre_final_position += pre_position_displace[i] * pre_morph_weights[i];
        }
        WorldPos = (local_model * vec4(final_position, 1.0)).xyz;
        gl_Position  = projection * view * local_model * vec4(final_position, 1.0);
        Normal       = mat3(transpose(inverse(local_model))) * final_normal;

        ss_pos = projection * view * local_model * vec4(final_position, 1.0);
        pre_ss_pos = projection * pre_view * pre_local_model * vec4(pre_final_position, 1.0);
    }
    TexCoords     = texcoord;

}
