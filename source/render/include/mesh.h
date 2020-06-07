#pragma once

#include "render_core.h"
#include "texture.h"
#include "shader.h"

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texCoords;

    Vertex() = default;

    Vertex(const vec3 &position, const vec3 &normal, const vec2 &texCoords) {
        this->position = position;
        this->normal = normal;
        this->texCoords = texCoords;
    }
};

class Mesh {
public:
    Mesh() {}
    Mesh(const vector<Vertex> &vertices,
         const vector<GLuint> &indices);
    void Draw(Shader &shader, mat4 &model, mat4 &view, mat4 &projection);
    void Draw(Shader &shader, mat4 &model, mat4 &view, mat4 &projection,
            Texture &texture, vec3 color, bool useTexture, int nyu_label, int instance);

    void Release() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
    GLuint VAO;
    GLuint size;
private:
    GLuint VBO, EBO;
    void setupMesh(const vector<Vertex> &vertices,
                   const vector<GLuint> &indices);
};



