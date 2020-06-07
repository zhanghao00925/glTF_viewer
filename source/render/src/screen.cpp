//
// Created by hao on 5/16/17.
//

#include "screen.h"

GLfloat screenQuad[24] = {
        // Positions   // TexCoords
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
};

Screen::Screen() {
    // Create Screen Quad
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuad), &screenQuad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *) (2 * sizeof(GLfloat)));
    glBindVertexArray(0);
}

void Screen::Bind() {
}

void Screen::Release() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Screen::Draw(Shader shader, GLuint gPositionId, GLuint gNormalId, GLuint gAlbedoSpecId, GLuint gAuxBuffer) {
    shader.Use();
    // Bind Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPositionId);
    glUniform1i(glGetUniformLocation(shader.Program, "gPosition"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormalId);
    glUniform1i(glGetUniformLocation(shader.Program, "gNormal"), 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpecId);
    glUniform1i(glGetUniformLocation(shader.Program, "gAlbedoSpec"), 2);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, gAuxBuffer);
    glUniform1i(glGetUniformLocation(shader.Program, "gAuxBuffer"), 3);

    // Draw Screen
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}