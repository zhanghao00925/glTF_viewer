#include "mesh.h"

Mesh::Mesh(const vector<Vertex> &vertices,
           const vector<GLuint> &indices) {

    size = indices.size();
    setupMesh(vertices, indices);
}

void Mesh::Draw(Shader &shader, mat4 &model, mat4 &view, mat4 &projection) {
    shader.Use();

    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader, mat4 &model, mat4 &view, mat4 &projection,
        Texture &texture, vec3 color, bool useTexture, int nyu_label, int instance) {
    shader.Use();

    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    if (useTexture) {
        glUniform1i(glGetUniformLocation(shader.Program, "use_texture"), 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.texId);
        glUniform1i(glGetUniformLocation(shader.Program, "color_texture"), 0);
    } else {
        glUniform1i(glGetUniformLocation(shader.Program, "use_texture"), 0);
        glUniform3fv(glGetUniformLocation(shader.Program, "in_color"), 1, glm::value_ptr(color));
    }
    glUniform1i(glGetUniformLocation(shader.Program, "nyu_label"), nyu_label);
    glUniform1i(glGetUniformLocation(shader.Program, "instance"), instance);

    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh(const vector<Vertex> &vertices,
                     const vector<GLuint> &indices) {
    // Create
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind VAO
    glBindVertexArray(VAO);
    // Bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    // Bind EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    // Vertex Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    // Vertex texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));
    // Unbind
    glBindVertexArray(0);
}
