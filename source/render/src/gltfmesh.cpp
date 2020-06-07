/*************************/
/*  FILE NAME: mesh.cpp  */
/*************************/
#include "gltfmesh.h"

/**************/
/*  INCLUDES  */
/**************/

/* default constructor */
GLTFMesh::GLTFMesh()
        : name(), vertices(), indices(), material_id(), matrix(), joint_matrices(), vao(), vbo(), ebo() {
    for (unsigned int i = 0; i < MAX_NUM_JOINTS; ++i)
        joint_matrices[i] = glm::identity<mat4>();

    morph_indices = {0};
    morph_weights = {0};

    material_id = -1;
    vao = 0;
    vbo = 0;
    ebo = 0;
}

/* set the GLTFMesh data to be available in OpenGL */
void GLTFMesh::SetupMesh() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLTFVertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLTFVertex),
                          reinterpret_cast<void *>(offsetof(GLTFVertex, position)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLTFVertex),
                          reinterpret_cast<void *>(offsetof(GLTFVertex, normal)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLTFVertex),
                          reinterpret_cast<void *>(offsetof(GLTFVertex, texcoord)));

    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 4, GL_UNSIGNED_INT, sizeof(GLTFVertex),
                           reinterpret_cast<void *>(offsetof(GLTFVertex, joint)));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(GLTFVertex),
                          reinterpret_cast<void *>(offsetof(GLTFVertex, weight)));

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const int NUM_MORPH_TARGETS = morph_vertices.size();
    morph_vbos.resize(NUM_MORPH_TARGETS, 0);
    vector<vec3> Positions;
    for (int i = 0; i < NUM_MORPH_TARGETS; i++) {
        const int NUM_VERTICES = morph_vertices[i].size();
        Positions.clear();
        Positions.reserve(NUM_VERTICES);
        for (int j = 0; j < NUM_VERTICES; j++) {
            Positions.emplace_back(morph_vertices[i][j].position);
        }
        glGenBuffers(1, &morph_vbos[i]);
        glBindBuffer(GL_ARRAY_BUFFER, morph_vbos[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * Positions.size(), Positions.data(), GL_STATIC_DRAW);

    }
}

/* clean up the GLTFMesh data that was set up */
void GLTFMesh::CleanupMesh() {
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    for (auto &morph_vbo: morph_vbos) { ;
        glDeleteBuffers(1, &morph_vbo);
    }
}

/* render the GLTFMesh data */
void GLTFMesh::Render(Shader shader, bool is_animation, bool is_skin, vector<float> weights) {
    if (!morph_vbos.empty()) {
        // select
        pre_morph_indics = morph_indices;
        pre_morph_weights = morph_weights;
        for (int i = 0; i < MAX_NUM_MORPHS &&  i < morph_vbos.size(); i++) {
            float max = -1;
            int index = -1;
            // search
            for (int j = 0; j < weights.size(); j++) {
                if (max < weights[j]) {
                    max = weights[j];
                    index = j;
                }
            }
            // mark
            assert(index != -1);
            weights[index] = -1;
            // set vbo and weight buffer
            morph_indices[i] = index;
            morph_weights[i] = max;
        }
        glBindVertexArray(vao);
        for (int i = 0; i < MAX_NUM_MORPHS; i++) {
            glBindBuffer(GL_ARRAY_BUFFER, morph_vbos[morph_indices[i]]);
            glEnableVertexAttribArray(5 + i);
            glVertexAttribPointer(5 + i, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glBindBuffer(GL_ARRAY_BUFFER, morph_vbos[pre_morph_indics[i]]);
            glEnableVertexAttribArray(5 + i + MAX_NUM_MORPHS);
            glVertexAttribPointer(5 + i + MAX_NUM_MORPHS, 3, GL_FLOAT, GL_FALSE, 0, 0);
        }
        glBindVertexArray(0);
        glUniform1fv(glGetUniformLocation(shader.Program, "morph_weights"), MAX_NUM_MORPHS, morph_weights.data());
        glUniform1fv(glGetUniformLocation(shader.Program, "pre_morph_weights"), MAX_NUM_MORPHS, pre_morph_weights.data());

    }
    if (is_skin) {
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "bone_matrix"), MAX_NUM_JOINTS, GL_FALSE,
                           &joint_matrices[0][0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "pre_bone_matrix"), MAX_NUM_JOINTS, GL_FALSE,
                           &pre_joint_matrices[0][0][0]);
    } else {
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "local_model"), 1, GL_FALSE, &matrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "pre_local_model"), 1, GL_FALSE, &pre_matrix[0][0]);
    }

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, indices.size());
    glBindVertexArray(0);
}