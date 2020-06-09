/***********************/
/*  FILE NAME: mesh.h  */
/***********************/
#ifndef _MESH_H_
#define _MESH_H_

#include "render_core.h"
#include "shader.h"
/**************/
/*  INCLUDES  */
/**************/
#include <string>
#include <vector>
#include <array>
#include "vertex.h"

constexpr unsigned int MAX_NUM_JOINTS = 120U;
constexpr unsigned int MAX_NUM_MORPHS = 3U;

/**********************/
/*  CLASS NAME: Mesh  */
/**********************/
class Mesh
{
public:
    Mesh();
//    GLTFMesh(const GLTFMesh& other);

public:
    void SetupMesh();
    void CleanupMesh();
    void Render(Shader shader, bool is_skin, vector<float> weights = vector<float>());

public:
    std::string name;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    int material_id;
    mat4 matrix;
    mat4 pre_matrix;
    std::vector<std::vector<Vertex>> morph_vertices;

    std::array<int, MAX_NUM_MORPHS> morph_indices;
    std::array<int, MAX_NUM_MORPHS> pre_morph_indics;
    std::array<float, MAX_NUM_MORPHS> morph_weights;
    std::array<float, MAX_NUM_MORPHS> pre_morph_weights;
    std::array<mat4, MAX_NUM_JOINTS> joint_matrices;
    std::array<mat4, MAX_NUM_JOINTS> pre_joint_matrices;

private:
    vector<GLuint> morph_vbos;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
}; // class Mesh
#endif // !_MESH_H_