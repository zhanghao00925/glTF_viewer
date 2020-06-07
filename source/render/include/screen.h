#pragma once

#include "shader.h"

using namespace std;

class Screen {
public:
    Screen();
    void Bind();
    void Release();
    void Draw(Shader shader, GLuint gPositionId, GLuint gNormalId, GLuint gAlbedoSpecId, GLuint gAuxBuffer);

    GLuint VAO;
private:
    GLuint VBO;
};
